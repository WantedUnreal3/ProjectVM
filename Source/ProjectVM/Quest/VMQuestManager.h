// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameData/VMQuestData.h"
#include "Core/VMMonsterEnums.h"
#include "Core/VMItemEnums.h"
#include "UI/Quest/VMQuestDataObject.h"
#include "VMQuestManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestPublished, const FVMQuestData&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, const FVMQuestData&);
UCLASS()
class PROJECTVM_API UVMQuestManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	//NPC 한테 퀘스트 전달
	void AssignQuestToNPC(FName QuestId);
	
	//NPC 한테 퀘스트 완료 알림
	void CompleteQuestForNPC(FVMQuestData QuestData);

	// 퀘스트 수락
	void AcceptQuest(FVMQuestData QuestData);

	void ClearQuest(FVMQuestData QuestData);

	// 퀘스트 진행 알림
	void NotifyMonsterDeath(EMonsterName MonsterName);
	void NotifyItemCollecting();

	// 퀘스트 트래커 UI 갱신
	void UpdateQuestProgress(TArray<UVMQuestDataObject*> Quests);

public:
	FOnQuestPublished OnQuestPublished;
	FOnQuestCompleted OnQuestCompleted;

	//TMap의 값으로 TArray를 쓰려면 UPROPERTY를 사용하면 안됨. 리플렉션을 지원 못하기 때문. 그냥 C++로 사용해야함.
	TMap<FName, TArray<UVMQuestDataObject*>> CurrentQuests;
};
