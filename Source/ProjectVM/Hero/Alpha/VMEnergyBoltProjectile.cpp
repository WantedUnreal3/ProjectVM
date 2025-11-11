// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMEnergyBoltProjectile.h"
#include "Components/SplineComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Interface/VMStatChangeable.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


AVMEnergyBoltProjectile::AVMEnergyBoltProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	RootComponent = SplinePath;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(10.f);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AVMEnergyBoltProjectile::HitTarget);
	SphereCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel2);

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SphereMesh->SetupAttachment(SphereCollision);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMeshAsset.Succeeded())
	{
		SphereMesh->SetStaticMesh(SphereMeshAsset.Object);
		SphereMesh->SetWorldScale3D(FVector(0.1f));
	}

	SphereMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	/*EnergyBoltEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EnergyBoltEffect"));
	EnergyBoltEffect->SetupAttachment(RootComponent);
	EnergyBoltEffect->bAutoActivate = true;
	EnergyBoltEffect->SetAutoDestroy(true);*/

	Progress = 0.0f;
}

void AVMEnergyBoltProjectile::BindTarget(AActor* InTarget)
{
	if (InTarget == nullptr || InTarget->IsValidLowLevel() == false)
	{
		UE_LOG(LogTemp, Log, TEXT("InTarget is nullptr"));
		Destroy();

		return;
	}
	
	Target = InTarget;

	SplinePath->ClearSplinePoints();
	SplinePath->AddSplinePoint(GetActorLocation(), ESplineCoordinateSpace::World);
	SplinePath->AddSplinePoint(InTarget->GetActorLocation(), ESplineCoordinateSpace::World);
	SplinePath->UpdateSpline();
}

void AVMEnergyBoltProjectile::HitTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 몬스터 호출 로직
	IVMStatChangeable* StatChangeable = Cast<IVMStatChangeable>(OtherActor);

	if (StatChangeable)
	{
		UE_LOG(LogTemp, Log, TEXT("%s 충돌 !"), *OtherActor->GetName());
		StatChangeable->HealthPointChange(10.f, this);
	}

	Destroy();
}

void AVMEnergyBoltProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AVMEnergyBoltProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target == nullptr) return;

	Progress += DeltaTime / 10 * 5;
	float SplineLength = SplinePath->GetSplineLength();
	float TargetLength = SplineLength * Progress;
	
	FVector Pos = SplinePath->GetLocationAtDistanceAlongSpline(TargetLength, ESplineCoordinateSpace::World);
	SphereCollision->SetWorldLocation(Pos);


	if (Progress > 1.0f) Destroy();
}