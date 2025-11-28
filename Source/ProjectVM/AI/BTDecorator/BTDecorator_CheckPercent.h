// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckPercent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTDecorator_CheckPercent : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckPercent();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Check")
	int32 PercentToCheck;
};
