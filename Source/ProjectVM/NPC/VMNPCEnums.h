#pragma once
#include "CoreMinimal.h"
#include "VMNPCEnums.generated.h"

//NPC 종류
UENUM(BlueprintType)
enum class ENPCType : uint8
{
    None  UMETA(DisplayName = "없음"),           // 기본값
    Bob  UMETA(DisplayName = "마을 주민 밥"),   // 마을 주민 밥
    Buzz  UMETA(DisplayName = "상점 주인 바즈"),  // 상점 주인 바즈
    Anna  UMETA(DisplayName = "마법사 안나"),     // 마법사 안나
	Chicken UMETA(DisplayName = "래퍼 치킨")    // 래퍼 치킨
};

//NPC 대화 옵션
UENUM(BlueprintType)
enum class ENPCOption : uint8
{
    Talk        UMETA(DisplayName = "대화하기"),
    Quest       UMETA(DisplayName = "퀘스트 받기"),
    QuestClear  UMETA(DisplayName = "퀘스트 완료"),
    Shop        UMETA(DisplayName = "상점"),
    Exit        UMETA(DisplayName = "나가기"),
    Accept      UMETA(DisplayName = "수락"),
    Decline     UMETA(DisplayName = "거절")
};

UENUM(BlueprintType)
enum class ENPCState : uint8
{
    Idle	UMETA(DisplayName = "Idle"),
    Talk	UMETA(DisplayName = "Talk")
};