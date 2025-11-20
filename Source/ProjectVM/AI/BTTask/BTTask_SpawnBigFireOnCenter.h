// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpawnBigFireOnCenter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_SpawnBigFireOnCenter : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SpawnBigFireOnCenter();

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
