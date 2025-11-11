// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "VMQuestTrackerWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMQuestTrackerWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	// 블루프린트에서 바인딩
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestName;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* QuestSummary;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* QuestCheckBox;

	//리스트뷰가 데이터를 연결할 때 마다 호출됨
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	UFUNCTION()
	void UpdateUI();

private:

	UPROPERTY()
	class UVMQuestDataObject* QuestDataObject;
};
