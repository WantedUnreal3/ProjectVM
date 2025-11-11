// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameData/VMQuestData.h"
#include "VMQuestDataObject.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnOptionUpdated);

UCLASS()
class PROJECTVM_API UVMQuestDataObject : public UObject
{
	GENERATED_BODY()
	
public:
	void SetQuestTrackerText()
	{
		OnOptionUpdated.Broadcast(); //위젯에 텍스트 변경 알림.
	}
public:
	FOnOptionUpdated OnOptionUpdated;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FVMQuestData QuestData;

	UPROPERTY()
	int32 CurrentCount = 0;
};
