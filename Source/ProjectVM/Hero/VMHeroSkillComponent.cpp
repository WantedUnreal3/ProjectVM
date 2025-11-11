// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/VMHeroSkillComponent.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Hero/SkillBase.h"
#include "Hero/HeroStat.h"

UVMHeroSkillComponent::UVMHeroSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UVMHeroSkillComponent::ExecuteBasicSkill(FHeroStat& CurStat)
{
	if (BasicSkill == nullptr) return;
	if (BasicSkill->IsSkillValid(CurStat) == false) return;

	AVMCharacterHeroBase* Owner = Cast<AVMCharacterHeroBase>(GetOwner());
	BasicSkill->ActivateSkill(Owner, CurStat);
}

void UVMHeroSkillComponent::ExecuteAdvancedSkill(FHeroStat& CurStat)
{
	if (AdvancedSkill == nullptr) return;
	if (AdvancedSkill->IsSkillValid(CurStat) == false) return;

	//AdvancedSkill->ActivateSkill(CurStat);
}

void UVMHeroSkillComponent::ExecuteMovementSkill(FHeroStat& CurStat)
{
	if (MovementSkill == nullptr) return;
	if (MovementSkill->IsSkillValid(CurStat) == false) return;

	//MovementSkill->ActivateSkill(CurStat);
}

void UVMHeroSkillComponent::ExecuteUltimateSkill(FHeroStat& CurStat)
{
	if (UltimateSkill == nullptr) return;
	if (UltimateSkill->IsSkillValid(CurStat) == false) return;

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

