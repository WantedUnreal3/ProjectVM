// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_WarriorMoveDashToTarget.h"

// BTTask_GlideDashToTarget.cpp
#include "AIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_WarriorMoveDashToTarget::UBTTask_WarriorMoveDashToTarget()
{
    NodeName = TEXT("Glide Dash To Target");
}

EBTNodeResult::Type UBTTask_WarriorMoveDashToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UE_LOG(LogTemp, Log, TEXT("Glide Dash To Target"));
    AAIController* AICon = OwnerComp.GetAIOwner();
    ACharacter* OwnerChar = Cast<ACharacter>(AICon->GetPawn());
    if (!OwnerChar) return EBTNodeResult::Failed;

    AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("EnemyTarget"));
    if (!Target)
    {
        return EBTNodeResult::Failed;
    }

    FVector Start = OwnerChar->GetActorLocation();
    FVector End = Target->GetActorLocation();

    float X = (End - Start).X;
    float Y = (End - Start).Y;
    float Z = (End - Start).Z;
    UE_LOG(LogTemp, Log, TEXT("(%f, %f, %f)"), X, Y, Z);

    FVector Dir = (End - Start).GetSafeNormal2D();
    //FVector Dir = (End - Start).GetSafeNormal();

    float DashDistance = FMath::Min(FVector::Dist(Start, End), 200.0f);
    float DashTime = 0.2f; // 스르륵 속도 (초당 이동)
    float DashSpeed = DashDistance / DashTime;
    

    /*FVector CurVelocity = OwnerChar->GetVelocity();
    CurVelocity.X *= 10;
    CurVelocity.Y *= 10;
    CurVelocity.Z = 0;*/

    auto Move = OwnerChar->GetCharacterMovement();
    Move->bUseSeparateBrakingFriction = false;
    Move->FallingLateralFriction = 5.0f;
    OwnerChar->LaunchCharacter(Dir * DashSpeed, false, false);
    

    return EBTNodeResult::Succeeded;
}