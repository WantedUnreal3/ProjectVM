// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckPhaseLevel.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_CheckPhaseLevel::UBTDecorator_CheckPhaseLevel()
{
}

bool UBTDecorator_CheckPhaseLevel::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!BB)
		return false;

	// BlackBoard의 값 가져오기 (Int 형태로 저장되어 있다고 가정)
	int32 CurrentPhaseValue = BB->GetValueAsInt(TEXT("BossPhase"));
	EBossPhase CurrentPhase = static_cast<EBossPhase>(CurrentPhaseValue);

	// 비교
	return CurrentPhase == PhaseToCheck;
}

	

FString UBTDecorator_CheckPhaseLevel::GetStaticDescription() const
{
	return FString::Printf(TEXT("Check if Boss Phase == %s"), *UEnum::GetValueAsString(PhaseToCheck));
}