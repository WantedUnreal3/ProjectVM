// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "HeroStat.generated.h"

USTRUCT(BlueprintType)
struct FHeroStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 AttackPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 DefensivePower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 HealthPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 ManaPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 ManaRegeneration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 LifeSteal;
};