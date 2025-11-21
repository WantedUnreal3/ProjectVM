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
	void StartBasicSkillCooldown(float Time);
	void StartAdvancedSkillCooldown(float Time);
	void StartMovementSkillSkillCooldown(float Time);
	void StartUltimateSkillCooldown(float Time);
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void Cooldown(float DeltaTime);

protected:
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
	float BasicSkillCooldown;
	UPROPERTY()
	float AdvancedSkillCooldown;
	UPROPERTY()
	float MovementSkillCooldown;
	UPROPERTY()
	float UltimateSkillCooldown;
};
