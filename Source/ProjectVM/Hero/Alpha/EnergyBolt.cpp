// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/EnergyBolt.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"
#include "Hero/HeroStat.h"
#include "Hero/Alpha/VMEnergyBoltProjectile.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

#include "CollisionQueryParams.h"
#include "Macro/VMPhysics.h"

#include "Engine/World.h"
#include "TimerManager.h"


UEnergyBolt::UEnergyBolt(const FObjectInitializer& ObjectInitializer)
{
	SkillName = TEXT("EnergyBolt");
	SkillDesc = TEXT("에너지 볼트를 발사합니다.");
	ManaCost = 30;
	Cooldown = 2.0f;
	RemainingCooldown = 0.0f;

	ProjectileCount = 12;
}

void UEnergyBolt::ActivateSkill(AVMCharacterHeroBase* InOwner, UVMHeroStatComponent* StatComp)
{
	Super::ActivateSkill(InOwner, StatComp);

	if (InOwner == nullptr)
	{
		return;
	}

	if (Owner == nullptr)
	{
		Owner = InOwner;
	}

	if (Owner != nullptr && Owner != InOwner)
	{
		return;
	}

	FHeroStat CurStat = StatComp->GetStat();
	ProjectileDamage = 10 + CurStat.AttackPower / 10;
	
	FVector Center = Owner->GetActorLocation();
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(1000.0f);

	TArray<FOverlapResult> Overlaps;
	bool HitDetected = Owner->GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, VM_ENEMY_TARGET_ACTION, CollisionShape);
	
	FColor DrawDebugColor = FColor::Green;
	if (HitDetected) DrawDebugColor = FColor::Red;

	for (FOverlapResult& Target : Overlaps)
	{
		Targets.Add(Target.GetActor());
	}

	if (Targets.IsEmpty())
	{
		return;
	}

	StartSpawnProjectile();

#if HERO_SKILL_DEBUG
	DrawDebugSphere(Owner->GetWorld(), Center, 1000.0f, 32, DrawDebugColor, false, 2.f);
	UE_LOG(LogTemp, Log, TEXT("타겟을 %d 개 찾았습니다."), Overlaps.Num());
	for (const FOverlapResult& Target : Overlaps)
		UE_LOG(LogTemp, Log, TEXT("발견한 타겟 : %s"), *Target.GetActor()->GetName());
#endif
}

void UEnergyBolt::StartSpawnProjectile()
{
	ProjectileCountToSpawn += ProjectileCount;
	GetWorld()->GetTimerManager().SetTimer(ProjectileTimerHandle, this, &UEnergyBolt::SpawnProjectile, 0.1f, true);
}

void UEnergyBolt::SpawnProjectile()
{
	static int32 TargetIndex = 0;
	
	if (ProjectileCountToSpawn <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProjectileTimerHandle);
		TargetIndex = 0;
		Targets.Empty();
		return;
	}

	AActor* Target = Targets[TargetIndex % Targets.Num()];
	AVMEnergyBoltProjectile* Projectile = Owner->GetWorld()->SpawnActor<AVMEnergyBoltProjectile>(AVMEnergyBoltProjectile::StaticClass(), Owner->GetActorLocation(), FRotator::ZeroRotator);
	if (Projectile != nullptr && Projectile->IsValidLowLevel())
	{
		Projectile->InitProjectile(Owner, Target, ProjectileDamage);
		--ProjectileCountToSpawn;
		++TargetIndex;
	}
}
