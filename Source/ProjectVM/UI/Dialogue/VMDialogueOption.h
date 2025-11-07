// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "NPC/VMNPCEnums.h"
#include "VMDialogueOption.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMDialogueOption : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
	FString OptionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
	ENPCOption OptionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Option")
	TObjectPtr <class AVMNPC > OwnerNPC;
};
