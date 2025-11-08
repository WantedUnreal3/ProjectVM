// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsTeleportRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTDecorator_IsTeleportRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsTeleportRange();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
