// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FlyToDest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_FlyToDest : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FlyToDest();

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector DestPositionKey;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MoveSpeed = 300.0f; // 단위: cm/s
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AcceptanceRadius = 50.0f; // 목표 도착 판정 반경
	
};
