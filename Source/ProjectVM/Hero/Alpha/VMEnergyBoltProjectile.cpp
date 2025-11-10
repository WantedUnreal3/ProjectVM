// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMEnergyBoltProjectile.h"
#include "Components/SplineComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


AVMEnergyBoltProjectile::AVMEnergyBoltProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	RootComponent = SplinePath;

	SphereCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(10.f);

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
	Target = InTarget;

	SplinePath->ClearSplinePoints();
	SplinePath->AddSplinePoint(GetActorLocation(), ESplineCoordinateSpace::World);
	SplinePath->AddSplinePoint(InTarget->GetActorLocation(), ESplineCoordinateSpace::World);
	SplinePath->UpdateSpline();
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
	BoxCollision->SetWorldLocation(Pos);

	if (Progress > 1.0f) Destroy();
}

void AVMEnergyBoltProjectile::Homing()
{
}

