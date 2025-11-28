#pragma once
#include "CoreMinimal.h"
#include "VMHeroEnums.generated.h"

UENUM(BlueprintType)
enum class EHeroState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Move		UMETA(DisplayName = "Move"),
	Skill		UMETA(DisplayName = "Skill"),
	Dead 		UMETA(DisplayName = "Dead"),
};

