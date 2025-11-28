// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SelectAttackIndex.h"

#include "AIController.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "Projectile/VMStraightProjectile.h"
#include "Projectile/VMHomingProjectile.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SelectAttackIndex::UBTTask_SelectAttackIndex()
{
}

EBTNodeResult::Type UBTTask_SelectAttackIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    // AI ControllerPtr
    AAIController* AIControllerPtr = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIControllerPtr == nullptr)
    {
        return EBTNodeResult::Failed;
    }


    AVMEnemyBoss* BossPtr = Cast<AVMEnemyBoss>(AIControllerPtr->GetPawn());
    if (BossPtr == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[BTTask_ExplosiveKnockback::ExecuteTask] BossPtr is nullptr"));
        return EBTNodeResult::Failed;
    }


	int32 CurrentIndex = FMath::RandRange(MinimumIndex, MaximumIndex);
    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
    if (BBComp == nullptr)
    {
        return EBTNodeResult::Failed;
    }
    BBComp->SetValueAsInt(TEXT("RandIndex"), CurrentIndex);

    return EBTNodeResult::Succeeded;
}
