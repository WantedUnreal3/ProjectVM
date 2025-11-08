// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTDecorator_IsMpLow.h"
#include "BTDecorator_IsMpLow.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsMpLow::UBTDecorator_IsMpLow()
{
}

bool UBTDecorator_IsMpLow::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	float CurrentMpValue = 0;
	float MaxMpValue = 1;
	CurrentMpValue = BBCompPtr->GetValueAsFloat(TEXT("OwnerCurrentMp"));
	MaxMpValue = BBCompPtr->GetValueAsFloat(TEXT("OwnerMaxMp"));


	return (CurrentMpValue / MaxMpValue) <= 0.7f;
}
