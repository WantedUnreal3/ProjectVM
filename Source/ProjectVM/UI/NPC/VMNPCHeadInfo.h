// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMNPCHeadInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMNPCHeadInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> NPCName;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UImage> QuestionIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> ExclamationIcon;
};
