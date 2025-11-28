// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MeteorAttack.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_MeteorAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_MeteorAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	EBTNodeResult::Type SpawnMeteorToTarget(UBehaviorTreeComponent& OwnerComp, class AVMEnemyBoss* BossPtr);

private:
	void FireOneCircleProjectile(UBehaviorTreeComponent* OwnerComp, AVMEnemyBoss* BossPtr);

	FTimerHandle MeteorTimer;
	int32 SpawnTotalCount = 1;
	int32 SpawnFinishedCount = 0;
};
