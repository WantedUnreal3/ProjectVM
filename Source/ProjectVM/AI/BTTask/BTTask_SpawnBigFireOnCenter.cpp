// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SpawnBigFireOnCenter.h"
#include "AI/Enemies/VMEnemyBoss.h"

#include "AIController.h"
#include "Kismet/GameplayStatics.h"

#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "AOE/VMAOEFire.h"
#include "AOE/VMAOECenterFire.h"	// 이걸로 대체할 예정.

#include "Components/PrimitiveComponent.h"


UBTTask_SpawnBigFireOnCenter::UBTTask_SpawnBigFireOnCenter()
{
}

EBTNodeResult::Type UBTTask_SpawnBigFireOnCenter::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVMEnemyBoss* BossPawnPtr = Cast<AVMEnemyBoss>(AIControllerPtr->GetPawn());
	if (BossPawnPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	FTransform Transform = BossPawnPtr->GetActorTransform();
	AVMAOECenterFire* FireSpawnActor = GetWorld()->SpawnActor<AVMAOECenterFire>(AVMAOECenterFire::StaticClass(), Transform);
	if (FireSpawnActor == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("센터 불 소환 실패"));
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}
