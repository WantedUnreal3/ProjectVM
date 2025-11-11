// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_VMLaunchStraighProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_VMLaunchStraighProjectile : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_VMLaunchStraighProjectile();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
