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

	FVector TargetPos = PlayerPtr->GetActorLocation();
	TargetPos.Z += 200;
	UCharacterMovementComponent* MoveComp = Cast<UCharacterMovementComponent>(PawnPtr->GetMovementComponent());
	if (MoveComp)
	{
		MoveComp->StopMovementImmediately();
	}
	PawnPtr->SetActorLocation(TargetPos);

	return EBTNodeResult::Succeeded;
}
