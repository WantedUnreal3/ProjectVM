// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsMpLow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTDecorator_IsMpLow : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsMpLow();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
