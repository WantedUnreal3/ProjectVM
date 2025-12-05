// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMFireworks.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"
#include "Hero/VMAnimInstance.h"
#include "Hero/Alpha/VMFireworksEffect.h"
#include "Engine/OverlapResult.h"
#include "Macro/VMPhysics.h"

#include "GameFramework/SpringArmComponent.h"

UVMFireworks::UVMFireworks(const FObjectInitializer& ObjectInitializer)
{
	SkillName = TEXT("Fireworks");
	SkillDesc = TEXT("폭죽놀이로 광역 범위에 피해를 줍니다.");
	ManaCost = 50;
	Cooldown = 30.0f;
	RemainingCooldown = 0.0f;
}

void UVMFireworks::ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp)
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
	Damage = 100 + CurStat.AttackPower;

	UVMAnimInstance* AnimInstance = Cast<UVMAnimInstance>(InOwner->GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnSkillMotionStart.AddUObject(this, &UVMFireworks::StartWideshot);
		AnimInstance->OnSpawnProjectile.AddUObject(this, &UVMFireworks::SpawnEffect);
		AnimInstance->OnAttackTiming.AddUObject(this, &UVMFireworks::AreaAttack);
		AnimInstance->OnSkillMotionEnd.AddUObject(this, &UVMFireworks::EndWideshot);
	}
}

void UVMFireworks::StartWideshot()
{
	Owner->SetHeroState(EHeroState::Skill);

	USpringArmComponent* CameraBoom = Owner->FindComponentByClass<USpringArmComponent>();
	CameraBoom->TargetArmLength = 1000.0f;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 666.0f));
}

void UVMFireworks::SpawnEffect()
{
	FVector SpawnLocation = Owner->GetMesh()->GetSocketLocation(TEXT("SkillSpawnLocation"));
	Effect = Owner->GetWorld()->SpawnActor<AVMFireworksEffect>(AVMFireworksEffect::StaticClass(), SpawnLocation, Owner->GetActorRotation());
}

void UVMFireworks::AreaAttack()
{
	FVector Center = Effect->GetActorLocation();
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(2000.0f);

	TArray<FOverlapResult> Overlaps;
	bool HitDetected = Owner->GetWorld()->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, VM_ENEMY_TARGET_ACTION, CollisionShape);

	if (HitDetected == false) return;

	for (FOverlapResult& Target : Overlaps)
	{
		IVMStatChangeable* StatChangeable = Cast<IVMStatChangeable>(Target.GetActor());

		if (StatChangeable)
		{
			StatChangeable->HealthPointChange(Damage, Owner);
		}
	}
}

void UVMFireworks::EndWideshot()
{
	Owner->SetHeroState(EHeroState::Idle);

	USpringArmComponent* CameraBoom = Owner->FindComponentByClass<USpringArmComponent>();
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
}
