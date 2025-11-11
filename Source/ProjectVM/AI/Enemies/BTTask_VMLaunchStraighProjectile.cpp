// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/BTTask_VMLaunchStraighProjectile.h"
#include "BTTask_VMLaunchStraighProjectile.h"


#include "AIController.h"
#include "Projectile/VMStraightProjectile.h"


UBTTask_VMLaunchStraighProjectile::UBTTask_VMLaunchStraighProjectile()
{
}

EBTNodeResult::Type UBTTask_VMLaunchStraighProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Pawn = AIController->GetPawn();
	if (Pawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	

	FTransform spawnTransform = Pawn->GetActorTransform();
	FVector CurLocation = spawnTransform.GetLocation();
	CurLocation += Pawn->GetActorForwardVector() * 500;
	spawnTransform.SetLocation(CurLocation);

	AVMStraightProjectile* Projectile = GetWorld()->SpawnActor<AVMStraightProjectile>(AVMStraightProjectile::StaticClass(), spawnTransform);

	return EBTNodeResult::Succeeded;
}
