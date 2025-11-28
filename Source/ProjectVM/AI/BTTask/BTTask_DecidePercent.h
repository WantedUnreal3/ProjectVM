// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DecidePercent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_DecidePercent : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_DecidePercent();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = "Check")
	int32 MinimumPercent = 0;

	UPROPERTY(EditAnywhere, Category = "Check")
	int32 MaximumPercent = 100;
};
