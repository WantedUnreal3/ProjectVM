// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTTask_TeleportToOwner.h"
#include "BTTask_TeleportToOwner.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/VMPlayer.h"
#include "Hero/VMCharacterHeroBase.h"

#include "GameFramework/CharacterMovementComponent.h"

UBTTask_TeleportToOwner::UBTTask_TeleportToOwner()
{
}

EBTNodeResult::Type UBTTask_TeleportToOwner::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	
	UBlackboardComponent* BBCompPtr = OwnerComp.GetBlackboardComponent();
	if (BBCompPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UObject* ObjectPtr = BBCompPtr->GetValueAsObject(TEXT("MasterTarget"));
	if (ObjectPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	
	AVMCharacterHeroBase* PlayerPtr = Cast<AVMCharacterHeroBase>(ObjectPtr);
	if (PlayerPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	/*FVector TargetPos = PlayerPtr->GetActorLocation();
	UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(PawnPtr->GetMovementComponent());
	if (MoveComp)
	{
		MoveComp->StopMovementImmediately();
	}
	PawnPtr->SetActorLocation(TargetPos);*/

	FVector TargetPos = PlayerPtr->GetActorLocation();
	FVector MyPos = PawnPtr->GetActorLocation();

	// 내가 있는 위치에서 → Player 방향 벡터
	FVector Dir = (TargetPos - MyPos).GetSafeNormal();

	// Player 위치에서 100cm 뒤로 밀린 지점
	FVector OffsetPos = TargetPos - Dir * 100.0f;

	UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(PawnPtr->GetMovementComponent());
	if (MoveComp)
	{
		MoveComp->StopMovementImmediately();
	}

	// 최종 위치 이동
	PawnPtr->SetActorLocation(OffsetPos);

	return EBTNodeResult::Succeeded;
}
