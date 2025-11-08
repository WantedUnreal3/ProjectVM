// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsMinimumOnwerRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTDecorator_IsMinimumOnwerRange : public UBTDecorator
{
	GENERATED_BODY()
	

public:
	UBTDecorator_IsMinimumOnwerRange();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
