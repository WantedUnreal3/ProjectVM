#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VMNPCData.generated.h"

USTRUCT(BlueprintType)
struct FVMNPCData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FVMNPCData()
		: StartTalkId(NAME_None)
		, Type(NAME_None)
		, NPCName(FText::GetEmpty())
	{
	}

	//Getter
	//FORCEINLINE FName GetNPCId() const { return NPCId; }
	FORCEINLINE FName GetStartTalkId() const { return StartTalkId; }
	FORCEINLINE FName GetType() const { return Type; }
	FORCEINLINE FText GetNPCName() const { return NPCName; }

	//Setter
	//FORCEINLINE void SetNPCId(FName InNPCId) { NPCId = InNPCId; }
protected:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	//FName NPCId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName StartTalkId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FName Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	FText NPCName;

};