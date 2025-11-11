#pragma once
#include "CoreMinimal.h"
#include "GameEnums.generated.h"

UENUM(BlueprintType)
enum class EKeyType : uint8
{
	J	UMETA(DisplayName = "J"),
	E	UMETA(DisplayName = "E"),
	ESC	UMETA(DisplayName = "ESC"),
};

//입력매핑 컨텍스트 열거형
UENUM(BlueprintType)
enum class EInputMode : uint8
{
	Default		UMETA(DisplayName = "Default"),		// 기본
	Dialogue	UMETA(DisplayName = "Dialogue")		// 대화
};

//스크린 UI 열거형
UENUM(BlueprintType)
enum class EScreenUIType : uint8
{
	GameScreen		UMETA(DisplayName = "GameScreen"),		// 게임 스크린
	InventoryScreen UMETA(DisplayName = "InventoryScreen"),	// 인벤토리 스크린
	DialogueScreen	UMETA(DisplayName = "DialogueScreen")	// 대화 스크린
};