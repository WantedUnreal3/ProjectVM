// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsHpLow.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTDecorator_IsHpLow : public UBTDecorator
{
	GENERATED_BODY()
	
	
public:
	UBTDecorator_IsHpLow();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
