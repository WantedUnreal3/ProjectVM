// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMBurningFuse.h"

#include "Camera/CameraComponent.h"
#include "Hero/VMAnimInstance.h"
#include "Hero/Alpha/VMBurningFuseProjectile.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"

UVMBurningFuse::UVMBurningFuse(const FObjectInitializer& ObjectInitializer)
{
	SkillName = TEXT("BurningFuse");
	SkillDesc = TEXT("직선상으로 불타는 투사체를 발사합니다.");
	ManaCost = 5;
	Cooldown = 0.5f;
	RemainingCooldown = 0.0f;
}

void UVMBurningFuse::ActivateSkill(AVMCharacterHeroBase* InOwner, UVMHeroStatComponent* StatComp)
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

	if (Owner !=nullptr && Owner != InOwner)
	{
		return;
	}

	FHeroStat CurStat = StatComp->GetStat();
	ProjectileDamage = 50 + CurStat.AttackPower / 2;

	UVMAnimInstance* AnimInstance = Cast<UVMAnimInstance>(InOwner->GetMesh()->GetAnimInstance());
	if (AnimInstance == nullptr)
	{
		SpawnProjectile();	
	}
	else
	{
		AnimInstance->OnSkillMotionStart.AddUObject(this, &UVMBurningFuse::SpawnProjectile);
	}
}

void UVMBurningFuse::SpawnProjectile()
{
	FVector CameraForward = Owner->GetCameraComponent()->GetForwardVector();
	FVector ProjectileDirection = CameraForward;
	ProjectileDirection.Z = 0;
	ProjectileDirection.Normalize();
	
	AVMBurningFuseProjectile* Projectile = Owner->GetWorld()->SpawnActor<AVMBurningFuseProjectile>(AVMBurningFuseProjectile::StaticClass(), Owner->GetActorLocation(), ProjectileDirection.Rotation());
	if (Projectile != nullptr && Projectile->IsValidLowLevel())
	{
		Projectile->InitProjectile(Owner, ProjectileDamage);
	}
}
