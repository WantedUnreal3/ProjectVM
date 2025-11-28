// Fill out your copyright notice in the Description page of Project Settings.

#include "Hero/Alpha/VMBurningFuseProjectile.h"
#include "Components/SphereComponent.h"
#include "Interface/VMStatChangeable.h"

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
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	
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