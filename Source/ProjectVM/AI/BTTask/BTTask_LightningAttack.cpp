// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_LightningAttack.h"

#include "AIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_LightningAttack.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "AOE/VMAOELightning.h"
#include "AOE/VMAOEMeteor.h"
#include "AOE/VMAOEFrozen.h"

#include "Hero/VMCharacterHeroBase.h"

UBTTask_LightningAttack::UBTTask_LightningAttack()
{

}

EBTNodeResult::Type UBTTask_LightningAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    
    return SpawnThunderToTarget(OwnerComp, BossPtr);
}

EBTNodeResult::Type UBTTask_LightningAttack::SpawnThunderToTarget(UBehaviorTreeComponent& OwnerComp, AVMEnemyBoss* BossPtr)
{
    if (BossPtr == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    UWorld* World = BossPtr->GetWorld();
    if (World == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    // 초기화
    SpawnFinishedCount = 0;
    //TotalShots = 30;

    

    // 나머지는 타이머로 순차 발사
    World->GetTimerManager().SetTimer(ThunderTimer,
        [this, &OwnerComp, BossPtr, World]()
        {
            if (!BossPtr || !World)
            {
                World->GetTimerManager().ClearTimer(ThunderTimer);
                return;
            }

            // 매 발사 시점에 랜덤 X,Y 생성
            UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
            if (BBComp == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }

            AVMCharacterHeroBase* Target =  Cast<AVMCharacterHeroBase>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));
            if (Target == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }


            FTransform Transform = Target->GetActorTransform();

            FActorSpawnParameters Params;
            Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            //AVMAOEMeteor* Thunder = World->SpawnActorDeferred<AVMAOEMeteor>(AVMAOEMeteor::StaticClass(), Transform, BossPtr, BossPtr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
            AVMAOEFrozen* Thunder = World->SpawnActorDeferred<AVMAOEFrozen>(AVMAOEFrozen::StaticClass(), Transform, BossPtr, BossPtr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
            //AVMAOELightning* Thunder = World->SpawnActorDeferred<AVMAOELightning>(AVMAOELightning::StaticClass(), Transform, BossPtr, BossPtr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
            if (Thunder == nullptr)
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
                return;
            }

            Thunder->SetDelay(3.0f);
            Thunder->FinishSpawning(Transform);


                //SpawnActor<AVMAOELightning>(AVMAOELightning::StaticClass(), SpawnLocation, SpawnRotation, Params);

            SpawnFinishedCount++;

            // 마지막 발사 후 종료
            if (SpawnFinishedCount >= SpawnTotalCount)
            {
                World->GetTimerManager().ClearTimer(ThunderTimer);
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            }
        },
        0.3f,
        true
    );

    return EBTNodeResult::InProgress;
}
