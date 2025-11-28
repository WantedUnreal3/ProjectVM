// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMSparksFlyProjectile.h"
#include "Components/SplineComponent.h"
#include "Components/SphereComponent.h"

#include "Interface/VMStatChangeable.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Utils/VMMath.h"

AVMSparksFlyProjectile::AVMSparksFlyProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	RootComponent = SplinePath;
	SplinePath->bDrawDebug = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(1.f);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AVMSparksFlyProjectile::HitTarget);
	
	SparksFlyEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SparksFlyEffect"));
	SparksFlyEffect->SetupAttachment(SphereCollision);
	SparksFlyEffect->SetAutoActivate(true);
	//SparksFlyEffect->SetAutoDestroy(true);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Game/_SplineVFX/NS/NS_Spline_EnergyLoop_Burning.NS_Spline_EnergyLoop_Burning"));
	if (NiagaraSystemAsset.Succeeded())
	{
		SparksFlyEffect->SetAsset(NiagaraSystemAsset.Object);
		
		SparksFlyEffect->SetVariableFloat(TEXT("_ColorHue"), 0.1f);
		SparksFlyEffect->SetVariableFloat(TEXT("_Size"), 5.0f);
		SparksFlyEffect->SetVariableFloat(TEXT("_Speed"), 4.0f);
		SparksFlyEffect->SetVariableFloat(TEXT("Count"), 200.0f);
		SparksFlyEffect->SetVariableFloat(TEXT("Extent"), 10.0f);
		SparksFlyEffect->SetVariableFloat(TEXT("Noise"), 99.0f);
		SparksFlyEffect->SetVariableFloat(TEXT("Progress"), 1.0f);
		
		SparksFlyEffect->SetVariableBool(TEXT("AddDetail"), true);
		SparksFlyEffect->SetVariableBool(TEXT("LightON"), false);
		SparksFlyEffect->SetVariableBool(TEXT("withHead"), true);
	}
	
	Progress = 0.0f;
}

void AVMSparksFlyProjectile::InitProjectile(AActor* InOwner, AActor* InTarget, int32 InDamage)
{
	if (InOwner == nullptr || InOwner->IsValidLowLevel() == false)
	{
		Destroy();
		return;
	}
	
	if (InTarget == nullptr || InTarget->IsValidLowLevel() == false)
	{
		Destroy();
		return;
	}
	
	SetOwner(InOwner);
	Target = InTarget;
	Damage = FMath::Max(0, InDamage);

	CreateProjectilePath(InOwner, InTarget);
}

void AVMSparksFlyProjectile::HitTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != Target) return;
	
	IVMStatChangeable* StatChangeable = Cast<IVMStatChangeable>(OtherActor);

	if (StatChangeable)
	{
		StatChangeable->HealthPointChange(Damage, Owner);
	}

	SphereCollision->SetActive(false);

#if HERO_SKILL_DEBUG
	if (StatChangeable)
	{
		UE_LOG(LogTemp, Log, TEXT("타겟 %s 충돌 !"), *OtherActor->GetName());
	}
#endif
}

void AVMSparksFlyProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AVMSparksFlyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Target == nullptr)
	{
		Destroy();
		return;
	}

	Progress += DeltaTime * 2;
	float SplineLength = SplinePath->GetSplineLength();
	float TargetLength = SplineLength * Progress;
	
	FVector Pos = SplinePath->GetLocationAtDistanceAlongSpline(TargetLength, ESplineCoordinateSpace::World);
	SphereCollision->SetWorldLocation(Pos);

	SparksFlyEffect->SetVariableVec3(TEXT("CollisionPos"), Pos);
	
	if (Progress > 3.0f) Destroy();
}

void AVMSparksFlyProjectile::CreateProjectilePath(AActor* InOwner, AActor* InTarget)
{
	SplinePath->ClearSplinePoints();

	FVector StartLocation = InOwner->GetActorLocation();
	FVector EndLocation = InTarget->GetActorLocation();

	FVector CurveLocation = StartLocation + (EndLocation - StartLocation) * FMath::FRandRange(0.1f, 0.3f);
	float RandomRadius = FMath::FRandRange(100.0f, 250.0f);
	float RandomTheta = FMath::FRandRange(0.0f, PI);

	FCircle3D Circle(CurveLocation, EndLocation - StartLocation, RandomRadius);
	CurveLocation = Circle.GetLocation(RandomTheta);
	
	SplinePath->AddSplinePoint(StartLocation, ESplineCoordinateSpace::World);
	SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);
	
	SplinePath->AddSplinePoint(CurveLocation, ESplineCoordinateSpace::World);
	SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Curve);
	
	SplinePath->AddSplinePoint(EndLocation, ESplineCoordinateSpace::World);
	SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Linear);
	
	SplinePath->UpdateSpline();

#if HERO_SKILL_DEBUG
	DrawDebugCircle(GetWorld(), Circle.Center, Circle.Radius, 32, FColor::Green, false, 3.f, 0, 1.f, Circle.AxisX, Circle.AxisY, true);
#endif
}