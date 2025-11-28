// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_WallDown.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_WallDown : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_WallDown();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
