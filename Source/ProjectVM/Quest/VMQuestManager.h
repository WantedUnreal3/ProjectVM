// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameData/VMQuestData.h"
#include "VMQuestManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestPublished, FVMQuestData);
UCLASS()
class PROJECTVM_API UVMQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//NPC
	void AssignQuestToNPC(FName QuestId);
public:
	FOnQuestPublished OnQuestPublished;
};
