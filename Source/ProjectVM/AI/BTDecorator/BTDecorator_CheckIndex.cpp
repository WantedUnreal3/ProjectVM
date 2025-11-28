// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator/BTDecorator_CheckIndex.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckIndex::UBTDecorator_CheckIndex()
{
}

bool UBTDecorator_CheckIndex::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return false;
	}

	// BlackBoard의 값 가져오기 (Int 형태로 저장되어 있다고 가정)
	int32 CurrentPhaseValue = BBComp->GetValueAsInt(TEXT("RandIndex"));

	// 비교
	return CurrentPhaseValue == IndexToCheck;
}
