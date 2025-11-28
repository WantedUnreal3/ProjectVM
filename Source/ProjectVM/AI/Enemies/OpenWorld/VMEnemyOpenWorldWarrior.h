// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/VMEnemyBase.h"
#include "Core/VMMonsterEnums.h"
#include "VMEnemyOpenWorldWarrior.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMEnemyOpenWorldWarrior : public AVMEnemyBase
{
	GENERATED_BODY()
	
public:
	AVMEnemyOpenWorldWarrior();

	virtual void HealthPointChange(float Amount, AActor* Causer) override;

protected:
	EMonsterName MonsterName = EMonsterName::Warrior;
};
