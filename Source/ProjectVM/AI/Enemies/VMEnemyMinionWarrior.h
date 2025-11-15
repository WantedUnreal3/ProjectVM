// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/VMEnemyBase.h"
#include "VMEnemyMinionWarrior.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMEnemyMinionWarrior : public AVMEnemyBase
{
	GENERATED_BODY()
	
public:
	AVMEnemyMinionWarrior();

	virtual float GetAIMoveSpeed() const { return MoveSpeed; }
	virtual float GetAIAttackRange() const { return AttackRange; }
	virtual float GetAITurnSpeed() const { return TurnSpeed; }

private:
	float MoveSpeed = 200.0f;
	float AttackSpeed = 1.0f;
	float AttackRange = 200.0f;
	float TurnSpeed = 2.0f;

};
