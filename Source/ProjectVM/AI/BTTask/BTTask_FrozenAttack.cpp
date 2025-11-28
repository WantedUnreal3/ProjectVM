// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_FrozenAttack.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_LightningAttack.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "AOE/VMAOEFrozen.h"

#include "Hero/VMCharacterHeroBase.h"

UBTTask_FrozenAttack::UBTTask_FrozenAttack()
{
    
}

EBTNodeResult::Type UBTTask_FrozenAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIControllerPtr = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIControllerPtr == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[UBTTask_FireStraightProjectile::ExecuteTask] AIController is nullptr"));
        return EBTNodeResult::Failed;
    }


    AVMEnemyBoss* BossPtr = Cast<AVMEnemyBoss>(AIControllerPtr->GetPawn());
    if (BossPtr == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("[UBTTask_FireStraightProjectile::ExecuteTask] BossPtr is nullptr"));
        return EBTNodeResult::Failed;
    }

    UE_LOG(LogTemp, Log, TEXT("왔으면 좋겠네1."));
    return SpawnFrozenToTarget(OwnerComp, BossPtr);
}

EBTNodeResult::Type UBTTask_FrozenAttack::SpawnFrozenToTarget(UBehaviorTreeComponent& OwnerComp, class AVMEnemyBoss* BossPtr)
{
    // 보스가 없는 경우(이미 위에서 걸리지긴 함)
    if (BossPtr == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    UWorld* World = BossPtr->GetWorld();
    // 월드가 없는 경우
    if (World == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    UE_LOG(LogTemp, Log, TEXT("왔으면 좋겠네."));
    // 초기화
    SpawnFinishedCount = 0;

    SpawnTotalCount = FMath::RandRange(1, 20);

    // 나머지는 타이머로 순차 발사
    World->GetTimerManager().SetTimer(FrozenTimer,
        [this, &OwnerComp, BossPtr, World]()
        {
            if (BossPtr == nullptr || World == nullptr)
            {
                World->GetTimerManager().ClearTimer(FrozenTimer);
                return;
            }

            UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
            if (BBComp == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }

            AVMCharacterHeroBase* HeroPawnTarget = Cast<AVMCharacterHeroBase>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));
            if (HeroPawnTarget == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }


            FTransform Transform = HeroPawnTarget->GetActorTransform();
            UE_LOG(LogTemp, Log, TEXT("Target Transform: (%f, %f, %f)"), Transform.GetLocation().X, Transform.GetLocation().Y, Transform.GetLocation().Z);

            FActorSpawnParameters Params;
            Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            AVMAOEFrozen* SpawnFrozenActorPtr = World->SpawnActorDeferred<AVMAOEFrozen>(AVMAOEFrozen::StaticClass(), Transform, BossPtr, BossPtr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
            if (SpawnFrozenActorPtr == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }
            //RemoveDynamic
            SpawnFrozenActorPtr->OnAOEFrozenOverlapActor.RemoveDynamic(HeroPawnTarget, &AVMCharacterHeroBase::OnHitFrozenByAOE);
            SpawnFrozenActorPtr->OnAOEFrozenOverlapActor.AddDynamic(HeroPawnTarget, &AVMCharacterHeroBase::OnHitFrozenByAOE);

            SpawnFrozenActorPtr->FinishSpawning(Transform);

            SpawnFinishedCount++;

            // 마지막 발사 후 종료
            if (SpawnFinishedCount >= SpawnTotalCount)
            {
                World->GetTimerManager().ClearTimer(FrozenTimer);
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            }
        },
        0.3f,
        true
    );

    return EBTNodeResult::InProgress;
}