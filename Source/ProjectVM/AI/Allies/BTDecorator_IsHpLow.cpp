// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTDecorator_IsHpLow.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsHpLow::UBTDecorator_IsHpLow()
{
}

bool UBTDecorator_IsHpLow::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
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

	float CurrentHpValue = 0;
	float MaxHpValue = 1;
	CurrentHpValue = BBCompPtr->GetValueAsFloat(TEXT("OwnerCurrentHp"));
	MaxHpValue = BBCompPtr->GetValueAsFloat(TEXT("OwnerMaxHp"));


	return (CurrentHpValue / MaxHpValue) <= 0.8f;
}
