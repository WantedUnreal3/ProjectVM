// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_VMLaunchHomingProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_VMLaunchHomingProjectile : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_VMLaunchHomingProjectile();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
