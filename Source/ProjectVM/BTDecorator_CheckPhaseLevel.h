// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "Enum/BossPhase.h"

#include "BTDecorator_CheckPhaseLevel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTDecorator_CheckPhaseLevel : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckPhaseLevel();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	/** 어떤 Phase와 비교할지 */
	UPROPERTY(EditAnywhere, Category = "Check")
	EBossPhase PhaseToCheck;

	virtual FString GetStaticDescription() const override;
};
