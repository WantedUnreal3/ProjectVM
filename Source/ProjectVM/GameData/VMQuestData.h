#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VMQuestData.generated.h"


USTRUCT(BlueprintType)
struct FVMQuestData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FVMQuestData()
		: QuestId(NAME_None)
		, QuestGiver(NAME_None)
		, NextQuestId(TEXT(""))
		, QuestCategory(NAME_None)
		, QuestTitle(FText::GetEmpty())
		, QuestSummary(FText::GetEmpty())
		, QuestType(NAME_None)
		, QuestTarget(NAME_None)
		, TargetCount(0)
		, Reward(NAME_None)
		, RewardCount(0)
	{
	}

public:
	// 퀘스트 고유 ID
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Quest")
	FName QuestId;

	// 퀘스트 제공자 (NPC 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestGiver;

	// 선행 퀘스트 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FString NextQuestId;

	// 퀘스트 카테고리 (메인, 서브, 튜토리얼 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestCategory;

	// 퀘스트 제목
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText QuestTitle;

	// 퀘스트 요약 설명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FText QuestSummary;

	// 퀘스트 타입 (수집, 처치, 배달 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestType;

	// 목표 대상 (몬스터, 아이템 이름 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName QuestTarget;

	// 목표 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 TargetCount = 0;

	// 보상 아이템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	FName Reward;

	// 보상 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quest")
	int32 RewardCount = 0;
};