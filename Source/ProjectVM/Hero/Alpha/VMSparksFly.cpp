// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMSparksFly.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"
#include "Hero/HeroStat.h"
#include "Hero/Alpha/VMSparksFlyProjectile.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

#include "CollisionQueryParams.h"
#include "Macro/VMPhysics.h"

#include "Engine/World.h"
#include "TimerManager.h"
#include "VMSparksFlyProjectile.h"
#include "Hero/VMAnimInstance.h"

UVMSparksFly::UVMSparksFly(const FObjectInitializer& ObjectInitializer)
{
	SkillName = TEXT("SparksFly");
	SkillDesc = TEXT("주변으로 튀는 불똥을 생성합니다.");
	ManaCost = 30;
	Cooldown = 2.0f;
	RemainingCooldown = 0.0f;

	ProjectileCount = 12;
}

void UVMSparksFly::ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp)
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

	UVMAnimInstance* AnimInstance = Cast<UVMAnimInstance>(InOwner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		StartSpawnProjectile();	
	}
	else
	{
		AnimInstance->OnSkillMotionStart.AddUObject(this, &UVMSparksFly::StartSpawnProjectile);
	}

#if HERO_SKILL_DEBUG
	DrawDebugSphere(Owner->GetWorld(), Center, 1000.0f, 32, DrawDebugColor, false, 2.f);
	UE_LOG(LogTemp, Log, TEXT("타겟을 %d 개 찾았습니다."), Overlaps.Num());
	for (const FOverlapResult& Target : Overlaps)
		UE_LOG(LogTemp, Log, TEXT("발견한 타겟 : %s"), *Target.GetActor()->GetName());
#endif
}

void UVMSparksFly::StartSpawnProjectile()
{
	ProjectileCountToSpawn += ProjectileCount;
	GetWorld()->GetTimerManager().SetTimer(ProjectileTimerHandle, this, &UVMSparksFly::SpawnProjectile, 0.1f, true);
}

void UVMSparksFly::SpawnProjectile()
{
	static int32 TargetIndex = 0;
	
	if (ProjectileCountToSpawn <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ProjectileTimerHandle);
		TargetIndex = 0;
		Targets.Empty();
		return;
	}

	if (Targets.IsEmpty())
	{
		return;
	}
	
	AActor* Target = Targets[TargetIndex % Targets.Num()];
	AVMSparksFlyProjectile* Projectile = Owner->GetWorld()->SpawnActor<AVMSparksFlyProjectile>(AVMSparksFlyProjectile::StaticClass(), Owner->GetActorLocation(), FRotator::ZeroRotator);
	if (Projectile != nullptr && Projectile->IsValidLowLevel())
	{
		Projectile->InitProjectile(Owner, Target, ProjectileDamage);
		--ProjectileCountToSpawn;
		++TargetIndex;
	}
}
