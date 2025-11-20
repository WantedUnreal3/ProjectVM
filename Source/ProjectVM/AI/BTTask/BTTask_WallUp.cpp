// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_WallUp.h"

#include "AIController.h"

#include "AI/Enemies/VMEnemyBoss.h"

#include "Environment/BossWall.h"



UBTTask_WallUp::UBTTask_WallUp()
{
}

EBTNodeResult::Type UBTTask_WallUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVMEnemyBoss* BossPawnPtr = Cast<AVMEnemyBoss>(AIControllerPtr->GetPawn());
	if (BossPawnPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	BossPawnPtr->BossWall->StartFireGimmick();
	return EBTNodeResult::Succeeded;
}

