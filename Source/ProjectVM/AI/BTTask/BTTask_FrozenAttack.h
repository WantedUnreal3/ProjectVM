// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FrozenAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_FrozenAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FrozenAttack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	EBTNodeResult::Type SpawnFrozenToTarget(UBehaviorTreeComponent& OwnerComp, class AVMEnemyBoss* BossPtr);

private:
	void FireOneCircleProjectile(UBehaviorTreeComponent* OwnerComp, AVMEnemyBoss* BossPtr);

	FTimerHandle FrozenTimer;
	int32 SpawnTotalCount = 1;
	int32 SpawnFinishedCount = 0;
};
