// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMSkillsCooldownWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMSkillsCooldownWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void StartBasicSkillCooldown(class USkillBase* Skill);
	void StartAdvancedSkillCooldown(class USkillBase* Skill);
	void StartMovementSkillCooldown(class USkillBase* Skill);
	void StartUltimateSkillCooldown(class USkillBase* Skill);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void UpdateBasicSkillCooldown(float DeltaTime);
	void UpdateAdvancedSkillCooldown(float DeltaTime);
	void UpdateMovementSkillCooldown(float DeltaTime);
	void UpdateUltimateSkillCooldown(float DeltaTime);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> BasicSkillCooldownProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> AdvancedSkillCooldownProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> MovementSkillCooldownProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> UltimateSkillCooldownProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> BasicSkillIcon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> AdvancedSkillIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> MovementSkillIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> UltimateSkillIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> BasicSkillCooldownText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AdvancedSkillCooldownText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> MovementSkillCooldownText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> UltimateSkillCooldownText;

	UPROPERTY()
	float BasicSkillCooldownBase;
	UPROPERTY()
	float AdvancedSkillCooldownBase;
	UPROPERTY()
	float MovementSkillCooldownBase;
	UPROPERTY()
	float UltimateSkillCooldownBase;

	UPROPERTY()
	float BasicSkillCooldownRemaining;
	UPROPERTY()
	float AdvancedSkillCooldownRemaining;
	UPROPERTY()
	float MovementSkillCooldownRemaining;
	UPROPERTY()
	float UltimateSkillCooldownRemaining;
};
