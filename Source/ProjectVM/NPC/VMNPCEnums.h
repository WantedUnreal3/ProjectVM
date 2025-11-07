#pragma once
#include "CoreMinimal.h"
#include "VMNPCEnums.generated.h"

//NPC 종류
UENUM(BlueprintType)
enum class ENPCType : uint8
{
    None  UMETA(DisplayName = "없음"),               // 기본값
    Bob   UMETA(DisplayName = "마을 주민 밥"),        // 마을 주민 Bob
    Sue   UMETA(DisplayName = "상점 주인 수"),        // 상점 주인 Sue
    Tom   UMETA(DisplayName = "퀘스트 제공 톰"),      // 퀘스트 제공 NPC Tom
    Anna  UMETA(DisplayName = "경비 안나"),           // 경비 Anna
    Joe   UMETA(DisplayName = "대장장이 조")          // 대장장이 Joe
};

//NPC 대화 옵션
UENUM(BlueprintType)
enum class ENPCOption : uint8
{
    Talk    UMETA(DisplayName = "대화하기"),
    Quest   UMETA(DisplayName = "퀘스트 받기"),
    Shop    UMETA(DisplayName = "상점"),
    Exit    UMETA(DisplayName = "나가기")
};