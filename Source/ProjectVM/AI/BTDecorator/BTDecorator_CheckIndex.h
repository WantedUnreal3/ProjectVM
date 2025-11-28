// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CheckIndex.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTDecorator_CheckIndex : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_CheckIndex();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, Category = "Check")
	int32 IndexToCheck;
	
};
