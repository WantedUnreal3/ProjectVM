// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LightningAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_LightningAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_LightningAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// case 1: 내 위치에 1발씩 쏘기.
	EBTNodeResult::Type SpawnThunderToTarget(UBehaviorTreeComponent& OwnerComp, class AVMEnemyBoss* BossPtr);

private:
	void FireOneCircleProjectile(UBehaviorTreeComponent* OwnerComp, AVMEnemyBoss* BossPtr);

	FTimerHandle ThunderTimer;
	int32 SpawnTotalCount = 1;
	int32 SpawnFinishedCount = 0;
};
