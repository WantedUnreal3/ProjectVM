// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/VMHeroSkillComponent.h"
#include "Hero/VMHeroStatComponent.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/SkillBase.h"
#include "Hero/HeroStat.h"

UVMHeroSkillComponent::UVMHeroSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVMHeroSkillComponent::ExecuteBasicSkill(AVMCharacterHeroBase* Owner, UVMHeroStatComponent* StatComp)
{
	if (BasicSkill == nullptr)
	{
		return;
	}
	
	if (BasicSkill->IsSkillValid(StatComp->GetStat()))
	{
		BasicSkill->ActivateSkill(Owner, StatComp);
	}
}

void UVMHeroSkillComponent::ExecuteAdvancedSkill(class UVMHeroStatComponent* StatComp)
{
	if (AdvancedSkill == nullptr) return;
	if (AdvancedSkill->IsSkillValid(StatComp->GetStat()) == false) return;

	//AdvancedSkill->ActivateSkill(CurStat);
}

void UVMHeroSkillComponent::ExecuteMovementSkill(class UVMHeroStatComponent* StatComp)
{
	if (MovementSkill == nullptr) return;
	if (MovementSkill->IsSkillValid(StatComp->GetStat()) == false) return;

	//MovementSkill->ActivateSkill(CurStat);
}

void UVMHeroSkillComponent::ExecuteUltimateSkill(class UVMHeroStatComponent* StatComp)
{
	if (UltimateSkill == nullptr) return;
	if (UltimateSkill->IsSkillValid(StatComp->GetStat()) == false) return;

	//UltimateSkill->ActivateSkill(CurStat);
}

void UVMHeroSkillComponent::BindBasicSkill(USkillBase* InSkill)
{
	BasicSkill = InSkill;
}

void UVMHeroSkillComponent::BindAdvancedSkill(USkillBase* InSkill)
{
	AdvancedSkill = InSkill;
}

void UVMHeroSkillComponent::BindMovementSkill(USkillBase* InSkill)
{
	MovementSkill = InSkill;
}

void UVMHeroSkillComponent::BindUltimateSkill(USkillBase* InSkill)
{
	UltimateSkill = InSkill;
}

void UVMHeroSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVMHeroSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BasicSkill != nullptr) BasicSkill->ReduceCooldown(DeltaTime);
}

