// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_CheckTargetDistance.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

//#include "Hero"

UBTDecorator_CheckTargetDistance::UBTDecorator_CheckTargetDistance()
{
}

bool UBTDecorator_CheckTargetDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return false;
	}

	// BlackBoard의 값 가져오기 (Int 형태로 저장되어 있다고 가정)
	APawn* TargetPawn = Cast<APawn>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));
	if (TargetPawn == nullptr)
	{
		return false;
	}

	float Distance = TargetPawn->GetDistanceTo(OwnerComp.GetAIOwner()->GetPawn());
	// 비교
	return Distance <= 1000;
}
