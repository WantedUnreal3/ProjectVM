#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VMNPCTalkData.generated.h"

USTRUCT(BlueprintType)
struct FVMNPCTalkData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FVMNPCTalkData()
		: Dialogue(TEXT(""))
		, bIsLastLine(false)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Talk")
	FString Dialogue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Talk")
	bool bIsLastLine;
};