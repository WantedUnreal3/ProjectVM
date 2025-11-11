#pragma once
#include "CoreMinimal.h"
#include "VMMonsterEnums.generated.h"


//몬스터 열거형
UENUM(BlueprintType)
enum class EMonsterName : uint8
{
	Warrior	UMETA(DisplayName = "Warrior"),	// 전사
	Archer	UMETA(DisplayName = "Archer"),	// 궁수
	Wizard	UMETA(DisplayName = "Wizard"),	// 마법사
	Minion	UMETA(DisplayName = "Minion"),	// 미니언
	Boss	UMETA(DisplayName = "Boss"),	// 보스
};