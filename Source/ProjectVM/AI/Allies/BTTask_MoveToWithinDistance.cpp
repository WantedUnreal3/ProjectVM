// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTTask_MoveToWithinDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTTask_MoveToWithinDistance::UBTTask_MoveToWithinDistance()
{
	bNotifyTick = true; // Tick 사용
}

EBTNodeResult::Type UBTTask_MoveToWithinDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
    if (AIControllerPtr == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("ExecuteTask 1"));
        return EBTNodeResult::Failed;
    }

    // Blackboard에서 TargetActor 가져오기
    UBlackboardComponent* BBCompPtr = OwnerComp.GetBlackboardComponent();
    if (BBCompPtr == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("ExecuteTask 2"));
        return EBTNodeResult::Failed;
    }


    TargetActor = Cast<AActor>(BBCompPtr->GetValueAsObject(TEXT("MasterTarget")));
 

    if (TargetActor == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("ExecuteTask 3"));
        return EBTNodeResult::Failed;
    }

    // MoveTo 시작
    AIControllerPtr->MoveToActor(TargetActor, 5.0f); // AcceptanceRadius는 임의로 5로 설정
    return EBTNodeResult::InProgress;
}

void UBTTask_MoveToWithinDistance::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    if (TargetActor == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("TickTask 1"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("TickTask 2"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    APawn* Pawn = AIController->GetPawn();
    if (Pawn == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("TickTask 3"));
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    // Target과 거리 체크
    float Distance = FVector::Dist(Pawn->GetActorLocation(), TargetActor->GetActorLocation());
    if (Distance <= StopDistance)
    {
        AIController->StopMovement(); // 이동 중단
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }
}
