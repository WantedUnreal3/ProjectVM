#pragma once
#include "CoreMinimal.h"
#include "VMItemEnums.generated.h"


//몬스터 열거형
UENUM(BlueprintType)
enum class EItemName : uint8
{
	Item1	UMETA(DisplayName = "Item1"),	// 아이템 1
	Item2	UMETA(DisplayName = "Item2"),	// 아이템 2
};