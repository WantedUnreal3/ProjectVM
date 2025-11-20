// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FlyToDest.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FlyToDest::UBTTask_FlyToDest()
{
    NodeName = "Fly To Destination";
    bNotifyTick = true; // TickTask 사용하려면 true로 설정
}

EBTNodeResult::Type UBTTask_FlyToDest::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Tick에서 이동 처리할 거라 바로 InProgress 반환
    return EBTNodeResult::InProgress;
}

void UBTTask_FlyToDest::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    AAIController* AICon = OwnerComp.GetAIOwner();
    if (AICon == nullptr)
    {
        return;
    }

    APawn* Pawn = AICon->GetPawn();
    if (Pawn == nullptr)
    {
        return;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp == nullptr)
    {
        return;
    }

    FVector Dest = BlackboardComp->GetValueAsVector(DestPositionKey.SelectedKeyName);
    FVector Current = Pawn->GetActorLocation();

    // 이동용 방향 (Z 포함)
    FVector MoveDir = Dest - Current;
    float Distance = MoveDir.Size();

    if (Distance <= AcceptanceRadius)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        return;
    }

    // 회전용 방향 (Z 제외)
    FVector LookDir = MoveDir;
    LookDir.Z = 0;
    if (!LookDir.IsNearlyZero())
    {
        FRotator TargetRot = LookDir.Rotation();
        Pawn->SetActorRotation(FRotator(0.f, TargetRot.Yaw, 0.f));
    }

    // 이동
    FVector MoveDelta = MoveDir.GetSafeNormal() * MoveSpeed * DeltaSeconds;
    Pawn->AddActorWorldOffset(MoveDelta, true);

    // 몽타주 재생 (원하면 추가)
}