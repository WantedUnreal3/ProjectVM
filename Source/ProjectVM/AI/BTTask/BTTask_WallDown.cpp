// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_WallDown.h"

#include "AIController.h"

#include "AI/Enemies/VMEnemyBoss.h"

#include "Environment/BossWall.h"

UBTTask_WallDown::UBTTask_WallDown()
{
}

EBTNodeResult::Type UBTTask_WallDown::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	BossPawnPtr->BossWall->EndFireGimmick();
	return EBTNodeResult::Succeeded;
}
