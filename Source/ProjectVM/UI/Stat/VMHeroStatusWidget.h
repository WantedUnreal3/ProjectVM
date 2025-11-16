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

	void RefreshHealthPointBar(float Percentage);
	void RefreshManaPointBar(float Percentage);

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HealthPointBar;
	
	UPROPERTY()
	TObjectPtr<class UProgressBar> ManaPointBar;
};
