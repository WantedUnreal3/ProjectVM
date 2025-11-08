// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToWithinDistance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_MoveToWithinDistance : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_MoveToWithinDistance();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
    /** 중단할 거리 */
    UPROPERTY(EditAnywhere, Category = "Move")
    float StopDistance = 300.0f;

private:
    /** 타겟 액터 */
    AActor* TargetActor = nullptr;
};
