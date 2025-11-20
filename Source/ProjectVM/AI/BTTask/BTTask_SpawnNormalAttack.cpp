// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SpawnNormalAttack.h"

#include "AIController.h"
#include "AI/Enemies/VMEnemySpawnBase.h"

UBTTask_SpawnNormalAttack::UBTTask_SpawnNormalAttack()
{
	bNotifyTick = true;
}


EBTNodeResult::Type UBTTask_SpawnNormalAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// EnemySpawn의 Pawn으로 변환 시도
	AVMEnemySpawnBase* EnemySpawnPawnPtr = Cast<AVMEnemySpawnBase>(AIControllerPtr->GetPawn());
	if (EnemySpawnPawnPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 애니메이션 재생.
	EnemySpawnPawnPtr->PlayNormalAttackMontage();

	return EBTNodeResult::InProgress;
}

void UBTTask_SpawnNormalAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	AVMEnemySpawnBase* Character = Cast<AVMEnemySpawnBase>(AIController->GetPawn());
	if (!Character)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
	if (AnimInstance->Montage_IsPlaying(Character->GetNormalMontage()) == true)
	{
		return;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}