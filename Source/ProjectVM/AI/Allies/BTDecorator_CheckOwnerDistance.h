// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckOwnerDistance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTDecorator_CheckOwnerDistance : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CheckOwnerDistance();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
