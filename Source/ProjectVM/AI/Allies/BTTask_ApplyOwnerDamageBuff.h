// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ApplyOwnerDamageBuff.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_ApplyOwnerDamageBuff : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_ApplyOwnerDamageBuff();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 우리의 함수
	void ResetDamage(class AVMPlayer* Character, float OriginalDamage);
};
