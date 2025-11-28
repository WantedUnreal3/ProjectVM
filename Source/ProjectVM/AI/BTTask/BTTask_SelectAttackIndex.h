// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SelectAttackIndex.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_SelectAttackIndex : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SelectAttackIndex();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


	UPROPERTY(EditAnywhere, Category = "Check")
	int32 MinimumIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Check")
	int32 MaximumIndex = 3;
};
