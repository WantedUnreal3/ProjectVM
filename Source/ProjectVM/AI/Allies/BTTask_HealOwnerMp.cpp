// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTTask_HealOwnerMp.h"

#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Macro/VMAIMarco.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "VMAllyBase.h"

UBTTask_HealOwnerMp::UBTTask_HealOwnerMp()
{
	bNotifyTick = true; // Tick 사용
}

EBTNodeResult::Type UBTTask_HealOwnerMp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	AVMAllyBase* Ally = Cast<AVMAllyBase>(PawnPtr);
	if (Ally == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Ally의 스킬을 사용
	Ally->HealOwnerMp();
	Ally->ActivateMpParticle();


	// 이벤트 발생으로 하기.
	UAnimInstance* AnimInstancePtr = Ally->GetMesh()->GetAnimInstance();
	if (AnimInstancePtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// TODO: 하드 코딩
	AnimInstancePtr->Montage_Play(Ally->HealMontage, 1.0f);

	return EBTNodeResult::InProgress;
}

void UBTTask_HealOwnerMp::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Log, TEXT("UBTTask_HealOwnerMp"));
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AVMAllyBase* AllyPtr = Cast<AVMAllyBase>(PawnPtr);
	if (AllyPtr == nullptr)
	{
		return;
	}

	if (AllyPtr->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AllyPtr->HealMontage) == true)
	{
		return;
	}

	AllyPtr->DeactivateMpParticle();
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}
