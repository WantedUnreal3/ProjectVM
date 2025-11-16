// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillBase.generated.h"

/**
 * 
 */

UCLASS(Abstract)
class PROJECTVM_API USkillBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void ActivateSkill(class AVMCharacterHeroBase* Owner, class UVMHeroStatComponent* StatComp);
	
	bool IsSkillValid(struct FHeroStat CurStat);
	void ReduceCooldown(float DeltaTime);
		
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	FString SkillName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	FString SkillDesc;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	int32 ManaCost;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	float Cooldown;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Skill, Meta = (AllowPrivateAccess = "true"))
	float RemainingCooldown;
};
