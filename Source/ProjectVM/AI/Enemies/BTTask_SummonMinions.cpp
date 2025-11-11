// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/BTTask_SummonMinions.h"

#include "AIController.h"

#include "VMEnemyBoss.h"

//#include "AI/NavigationSystemBase.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SummonMinions::UBTTask_SummonMinions()
{
	NodeName = TEXT("SummonMinions");
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_SummonMinions::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

void UBTTask_SummonMinions::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return ;
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

void UBTTask_SummonMinions::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
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
	AVMEnemyBoss* BossPtr = Cast<AVMEnemyBoss>(PawnPtr);
	if (BossPtr == nullptr)
	{
		return;
	}
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys == nullptr)
	{
		return;
	}

	FNavLocation RandomLocation;
	FVector MyPos;
	MyPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("InitPosition"));
	for (int i = 0; i < 10; ++i)
	{
		UE_LOG(LogTemp, Log, TEXT("(%f, %f, %f)"), MyPos.X, MyPos.Y, MyPos.Z);
		NavSys->GetRandomReachablePointInRadius(MyPos, 3000, RandomLocation);
		BossPtr->SummonMinion(RandomLocation.Location);
	}
}
