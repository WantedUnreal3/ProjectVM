// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMHeroStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMHeroStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UVMHeroStatusWidget(const FObjectInitializer& ObjectInitializer);

	void UpdateMaxHealthPoint(int InMaxHealthPoint);
	void UpdateCurrentHealthPoint(int InCurrentHealthPoint);
	void UpdateMaxManaPoint(int InMaxManaPoint);
	void UpdateCurrentManaPoint(int InCurrentManaPoint);

	void RefreshHealthPointBar(float Percentage);
	void RefreshManaPointBar(float Percentage);

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthPoint;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> ManaPoint;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HealthPointText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> ManaPointText;

	UPROPERTY()
	int MaxHealthPoint;
	UPROPERTY()
	int CurrentHealthPoint;

	UPROPERTY()
	int MaxManaPoint;
	UPROPERTY()
	int CurrentManaPoint;
};
