// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTDecorator_CheckOwnerDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckOwnerDistance::UBTDecorator_CheckOwnerDistance()
{
}

bool UBTDecorator_CheckOwnerDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return false;
	}

	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		return false;
	}

	UBlackboardComponent* BBCompPtr = OwnerComp.GetBlackboardComponent();
	if (BBCompPtr == nullptr)
	{
		return false;
	}

	float Distance = -1;
	Distance = BBCompPtr->GetValueAsFloat(TEXT("Distance"));


	return Distance >= 1000 && Distance < 2000;
}
