// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "VMEquipmentInfo.generated.h"

USTRUCT(BlueprintType)
struct FVMEquipmentInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemDesc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 DefensivePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealthPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ManaPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ManaRegeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 LifeSteal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Ability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 AtlasRow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 AtlasCol;
};