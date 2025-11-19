// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMBurningFuseProjectile.h"
#include "Components/SphereComponent.h"
#include "Interface/VMStatChangeable.h"

#include "Components/SplineComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Hero/VMCharacterHeroBase.h"

AVMBurningFuseProjectile::AVMBurningFuseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetSphereRadius(10.f);
	SphereCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AVMBurningFuseProjectile::HitTarget);
	SphereCollision->SetEnableGravity(false);
	SphereCollision->SetActive(true);
	
	RootComponent = SphereCollision;

	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileEffect"));
	ProjectileEffect->SetupAttachment(RootComponent);
	ProjectileEffect->SetAutoActivate(true);
	ProjectileEffect->SetAutoDestroy(false);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> ProjectileNiagaraSystemAsset(TEXT("/Game/ArrowTrail/FX/NS_ArrowTrail_Fire_02_Custom.NS_ArrowTrail_Fire_02_Custom"));
	if (ProjectileNiagaraSystemAsset.Succeeded())
	{
		ProjectileEffect->SetAsset(ProjectileNiagaraSystemAsset.Object);
	}

	/*
	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	SplinePath->SetupAttachment(RootComponent);
	SplinePath->ClearSplinePoints();
	SplinePath->bDrawDebug = true;

	BurningFuseEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BurningFuseEffect"));
	BurningFuseEffect->SetupAttachment(SplinePath);
	BurningFuseEffect->SetAutoActivate(true);
	BurningFuseEffect->SetAutoDestroy(false);

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> BurningFuseNiagaraSystemAsset(TEXT("/Game/_SplineVFX/NS/NS_Spline_Fire.NS_Spline_Fire"));
	if (BurningFuseNiagaraSystemAsset.Succeeded())
	{
		BurningFuseEffect->SetAsset(BurningFuseNiagaraSystemAsset.Object);

		BurningFuseEffect->SetVariableBool(TEXT("_AdaptTerrain"), false);
		BurningFuseEffect->SetVariableFloat(TEXT("_Brightness"), 3.0f);
		BurningFuseEffect->SetVariableFloat(TEXT("_ColorHue"), 0.0f);
		BurningFuseEffect->SetVariableFloat(TEXT("_Size"), 40.0f);
		BurningFuseEffect->SetVariableBool(TEXT("AddDetail"), true);
		BurningFuseEffect->SetVariableFloat(TEXT("Count(Density)"), 10.0f);
		BurningFuseEffect->SetVariableBool(TEXT("DecalON"), true);
		BurningFuseEffect->SetVariableFloat(TEXT("Extent"), 0.05f);
		BurningFuseEffect->SetVariableBool(TEXT("LightNo"), true);
		BurningFuseEffect->SetVariableBool(TEXT("Not_Type_A_But_B"), false);
		BurningFuseEffect->SetVariableFloat(TEXT("Progress"), 2.0f);
		BurningFuseEffect->SetVariableFloat(TEXT("Z_Decal_Threshold"), 150.0f);
	}*/
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	
	SplinePathProgress = 0.0f;
	DestroyProgress = 0.0f;
}

void AVMBurningFuseProjectile::InitProjectile(AActor* InOwner, int32 InDamage)
{
	SetOwner(InOwner);
	Owner = InOwner;
	Damage = InDamage;
}

void AVMBurningFuseProjectile::HitTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AVMCharacterHeroBase>(OtherActor) != nullptr)
	{
		return;
	}

	IVMStatChangeable* StatChangeable = Cast<IVMStatChangeable>(OtherActor);
	
	if (StatChangeable)
	{
		StatChangeable->HealthPointChange(Damage, Owner);
	}

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMovement->MaxSpeed = 0.0f;
	ProjectileEffect->SetVariableFloat(TEXT("SpawnRate"), 0.0f);

	GetWorld()->GetTimerManager().SetTimer(ProjectileDestroyHandle, this, &AVMBurningFuseProjectile::DestroyProjectile, 5.0f, false);

#if HERO_SKILL_DEBUG
	if (StatChangeable)
	{
		UE_LOG(LogTemp, Log, TEXT("타겟 %s 충돌 !"), *OtherActor->GetName());
	}
#endif
}

void AVMBurningFuseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AVMBurningFuseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	DestroyProgress += DeltaTime / 10;

	if (DestroyProgress > 1.0f)
	{
		DestroyProjectile();
	}
}

void AVMBurningFuseProjectile::DestroyProjectile()
{
	Destroy();
}

/*void AVMBurningFuseProjectile::AddProjectilePath()
{
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 10000.f);
	
	bool Hit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic);

	if (Hit)
	{
		SplinePath->AddSplinePoint(HitResult.Location, ESplineCoordinateSpace::World);
		SplinePath->SetSplinePointType(SplinePath->GetNumberOfSplinePoints(), ESplinePointType::Linear);	
		SplinePath->UpdateSpline();
	}
}*/

