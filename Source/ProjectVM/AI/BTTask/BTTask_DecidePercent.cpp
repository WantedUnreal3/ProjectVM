// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_DecidePercent.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DecidePercent::UBTTask_DecidePercent()
{
}

EBTNodeResult::Type UBTTask_DecidePercent::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // AI ControllerPtr
    AAIController* AIControllerPtr = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIControllerPtr == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    int32 CurrentIndex = FMath::RandRange(MinimumPercent, MaximumPercent - 1);
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (BBComp == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    BBComp->SetValueAsInt(TEXT("RandPercent"), CurrentIndex);

    return EBTNodeResult::Succeeded;
}
