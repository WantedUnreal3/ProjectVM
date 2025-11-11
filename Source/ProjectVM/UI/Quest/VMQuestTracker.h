// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMQuestTracker.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMQuestTracker : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> QuestTrackerListView;
};
