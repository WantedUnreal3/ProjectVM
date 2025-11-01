// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_EnemyDetect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTService_EnemyDetect : public UBTService
{
	GENERATED_BODY()

#pragma region 특수_맴버_함수
public:
	UBTService_EnemyDetect();
#pragma endregion

#pragma region 엔진_제공_함수
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
#pragma endregion
};
