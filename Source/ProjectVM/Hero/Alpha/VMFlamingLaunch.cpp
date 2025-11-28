// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMFlamingLaunch.h"
#include "Hero/Alpha/VMFlamingLaunchEffect.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"
#include "Hero/HeroStat.h"

UVMFlamingLaunch::UVMFlamingLaunch(const FObjectInitializer& ObjectInitializer)
{
	SkillName = TEXT("FlamingLaunch");
	SkillDesc = TEXT("캐릭터가 바라보는 방향으로 빠르게 대쉬합니다. 이동속도가 빠를수록 더 멀리 대쉬합니다.");
	ManaCost = 10;
	Cooldown = 2.0f;
	RemainingCooldown = 0.0f;
}
void UVMFlamingLaunch::ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp)
{
	Super::ActivateSkill(InOwner, StatComp);

	FVector Direction = (InOwner->GetActorForwardVector()).GetSafeNormal();
	InOwner->LaunchCharacter(Direction * (500 + StatComp->GetStat().Speed) + FVector(0.f, 0.f, 400.f), true, true);

	AVMFlamingLaunchEffect* Effect = InOwner->GetWorld()->SpawnActor<AVMFlamingLaunchEffect>(AVMFlamingLaunchEffect::StaticClass(), InOwner->GetActorLocation(), InOwner->GetActorRotation());
}
