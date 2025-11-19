// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMBurningFuse.h"

#include "Camera/CameraComponent.h"
#include "Hero/Alpha/VMBurningFuseProjectile.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"

UVMBurningFuse::UVMBurningFuse(const FObjectInitializer& ObjectInitializer)
{
	SkillName = TEXT("SparksFly");
	SkillDesc = TEXT("주변으로 튀는 불똥을 생성합니다.");
	ManaCost = 5;
	Cooldown = 0.5f;
	RemainingCooldown = 0.0f;
}

void UVMBurningFuse::ActivateSkill(AVMCharacterHeroBase* InOwner, UVMHeroStatComponent* StatComp)
{
	Super::ActivateSkill(InOwner, StatComp);
	UE_LOG(LogTemp, Log, TEXT("스킬 : BurningFuse"));

	FVector CameraForward = InOwner->GetCameraComponent()->GetForwardVector();
	FVector ProjectileDirection = CameraForward;
	ProjectileDirection.Z = 0;
	ProjectileDirection.Normalize();

	AVMBurningFuseProjectile* Projectile = InOwner->GetWorld()->SpawnActor<AVMBurningFuseProjectile>(AVMBurningFuseProjectile::StaticClass(), InOwner->GetActorLocation(), ProjectileDirection.Rotation());
	if (Projectile != nullptr && Projectile->IsValidLowLevel())
	{
		Projectile->InitProjectile(InOwner, 10 + StatComp->GetStat().AttackPower / 10);
	}
}
