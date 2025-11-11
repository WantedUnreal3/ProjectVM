// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/SkillBase.h"
#include "EnergyBolt.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UEnergyBolt : public USkillBase
{
	GENERATED_BODY()

public:
	UEnergyBolt(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateSkill(AVMCharacterHeroBase* Owner, struct FHeroStat& CurStat) override;
};