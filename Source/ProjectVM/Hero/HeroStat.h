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
	int32 AttackPower = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 DefensivePower = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 HealthPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 ManaPoint = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 ManaRegeneration = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 Speed = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeroStat")
	int32 LifeSteal = 0;
};