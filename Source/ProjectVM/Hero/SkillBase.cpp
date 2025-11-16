// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/SkillBase.h"

#include "VMHeroStatComponent.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/HeroStat.h"

void USkillBase::ActivateSkill(AVMCharacterHeroBase* Owner, UVMHeroStatComponent* StatComp)
{
	StatComp->ConsumeMana(ManaCost);
	RemainingCooldown = Cooldown;
}

bool USkillBase::IsSkillValid(FHeroStat CurStat)
{
	if (CurStat.ManaPoint < ManaCost) return false;
	if (RemainingCooldown > KINDA_SMALL_NUMBER) return false;
	
	return true;
}

void USkillBase::ReduceCooldown(float DeltaTime)
{
	if (RemainingCooldown <= KINDA_SMALL_NUMBER) return;

	RemainingCooldown = FMath::Max<float>(RemainingCooldown - DeltaTime, 0.0f);
	if (RemainingCooldown <= KINDA_SMALL_NUMBER) RemainingCooldown = 0.0f;
}