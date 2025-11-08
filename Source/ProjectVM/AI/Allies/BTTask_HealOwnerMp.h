// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_HealOwnerMp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_HealOwnerMp : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_HealOwnerMp();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
