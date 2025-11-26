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
		Owner->GetMesh()->GetAnimInstance()->Montage_Play(BasicSkillMontage);
		BasicSkill->ActivateSkill(Owner, StatComp);
		OnBasicSkillExecute.Broadcast(BasicSkill);
	}
}

void UVMHeroSkillComponent::ExecuteAdvancedSkill(AVMCharacterHeroBase* Owner, UVMHeroStatComponent* StatComp)
{
	if (AdvancedSkill == nullptr)
	{
		return;
	}
	
	if (AdvancedSkill->IsSkillValid(StatComp->GetStat()))
	{
		Owner->GetMesh()->GetAnimInstance()->Montage_Play(AdvancedSkillMontage);
		AdvancedSkill->ActivateSkill(Owner, StatComp);
		OnAdvancedSkillExecute.Broadcast(AdvancedSkill);
	}
}

void UVMHeroSkillComponent::ExecuteMovementSkill(AVMCharacterHeroBase* Owner, UVMHeroStatComponent* StatComp)
{
	if (MovementSkill == nullptr)
	{
		return;
	}
	
	if (MovementSkill->IsSkillValid(StatComp->GetStat()))
	{
		Owner->GetMesh()->GetAnimInstance()->Montage_Play(MovementSkillMontage);
		MovementSkill->ActivateSkill(Owner, StatComp);
		OnMovementSkillExecute.Broadcast(MovementSkill);
	}
}

void UVMHeroSkillComponent::ExecuteUltimateSkill(AVMCharacterHeroBase* Owner, UVMHeroStatComponent* StatComp)
{
	if (UltimateSkill == nullptr)
	{
		return;
	}
	
	if (UltimateSkill->IsSkillValid(StatComp->GetStat()))
	{
		Owner->GetMesh()->GetAnimInstance()->Montage_Play(UltimateSkillMontage);
		UltimateSkill->ActivateSkill(Owner, StatComp);
		OnUltimateSkillExecute.Broadcast(UltimateSkill);
	}
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

void UVMHeroSkillComponent::BindBasicSkillMontage(class UAnimMontage* InMontage)
{
	BasicSkillMontage = InMontage;
}

void UVMHeroSkillComponent::BindAdvancedSkillMontage(class UAnimMontage* InMontage)
{
	AdvancedSkillMontage = InMontage;
}

void UVMHeroSkillComponent::BindMovementSkillMontage(class UAnimMontage* InMontage)
{
	MovementSkillMontage = InMontage;
}

void UVMHeroSkillComponent::BindUltimateSkillMontage(class UAnimMontage* InMontage)
{
	UltimateSkillMontage = InMontage;
}

void UVMHeroSkillComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UVMHeroSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BasicSkill != nullptr) BasicSkill->ReduceCooldown(DeltaTime);
	if (AdvancedSkill != nullptr) AdvancedSkill->ReduceCooldown(DeltaTime);
	if (MovementSkill != nullptr) MovementSkill->ReduceCooldown(DeltaTime);
	if (UltimateSkill != nullptr) UltimateSkill->ReduceCooldown(DeltaTime);
}

