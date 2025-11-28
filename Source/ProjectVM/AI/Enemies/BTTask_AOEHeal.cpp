// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/BTTask_AOEHeal.h"

#include "AIController.h"
#include "AOE/VMAOEHeal.h"

UBTTask_AOEHeal::UBTTask_AOEHeal()
{
}

EBTNodeResult::Type UBTTask_AOEHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("UBTTask_AOEHeal!!!"));

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

	FTransform Transform = PawnPtr->GetActorTransform();
	AVMAOEHeal* HealSpawnedActor = GetWorld()->SpawnActorDeferred<AVMAOEHeal>(AVMAOEHeal::StaticClass(), Transform, PawnPtr, PawnPtr);
	if (HealSpawnedActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	HealSpawnedActor->FinishSpawning(Transform);
	return EBTNodeResult::Succeeded;
}
