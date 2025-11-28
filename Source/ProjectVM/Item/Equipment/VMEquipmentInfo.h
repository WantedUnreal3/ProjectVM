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
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMesh> Mesh = nullptr;

	UPROPERTY(EditAnywhere)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemName = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemDesc = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 AttackPower = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 DefensivePower = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 HealthPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ManaPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ManaRegeneration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 LifeSteal = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString Ability = TEXT("");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 AtlasRow = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 AtlasCol = 0;
};