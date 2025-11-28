// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/BTTask_HealAllMinionsAndBoss.h"

#include "AIController.h"
#include "EngineUtils.h"

#include "AI/Enemies/VMEnemySpawnBase.h"
#include "AI/Enemies/VMEnemyBoss.h"
UBTTask_HealAllMinionsAndBoss::UBTTask_HealAllMinionsAndBoss()
{
	NodeName = TEXT("HealMinions");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_HealAllMinionsAndBoss::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Animation을 실행.


	return EBTNodeResult::InProgress;
}

void UBTTask_HealAllMinionsAndBoss::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return;
	}

	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		return;
	}
	AVMEnemyBoss* BossPtr = Cast<AVMEnemyBoss>(PawnPtr);
	if (BossPtr == nullptr)
	{
		return;
	}

	UAnimInstance* AnimInstance = BossPtr->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}

	if (AnimInstance->Montage_IsPlaying(BossPtr->SummonMontage) == true)
	{
		return;
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Type::Succeeded);
}

void UBTTask_HealAllMinionsAndBoss::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	UE_LOG(LogTemp, Log, TEXT("UBTTask_SummonMinions::OnTaskFinished"));
	if (TaskResult == EBTNodeResult::Type::Aborted)
	{
		UE_LOG(LogTemp, Log, TEXT("이게 가능?Abort"));
		return;
	}
	if (TaskResult == EBTNodeResult::Type::InProgress)
	{
		UE_LOG(LogTemp, Log, TEXT("이게 가능?InProgress"));
		return;
	}
	if (TaskResult == EBTNodeResult::Type::Failed)
	{
		return;
	}

	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return;
	}

	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		return;
	}

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Enemy = *It;

		if (!Enemy || Enemy->ActorHasTag("Player"))
			continue;

		IEnemyHealInterface* ieh = Cast<IEnemyHealInterface>(Enemy);
		if (ieh)
		{
			ieh->OnHealHp(100.f);
		}
	}

}
