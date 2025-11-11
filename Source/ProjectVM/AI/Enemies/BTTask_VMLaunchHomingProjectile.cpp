// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/BTTask_VMLaunchHomingProjectile.h"

#include "AIController.h"
#include "Projectile/VMHomingProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/VMPlayer.h"

UBTTask_VMLaunchHomingProjectile::UBTTask_VMLaunchHomingProjectile()
{
}

EBTNodeResult::Type UBTTask_VMLaunchHomingProjectile::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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
	
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UObject* Obj = BBComp->GetValueAsObject(TEXT("EnemyTarget"));
	if (Obj == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyTarget is nullptr!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyTarget is valid: %s"), *Obj->GetName());
	}


	/*AVMPlayer* Player = Cast<AVMPlayer>(BBComp->GetValueAsObject(TEXT("EnemyTarget")));
	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("tq"));
	}*/

	/*FTransform spawnTransform = Pawn->GetActorTransform();
	FVector CurLocation = spawnTransform.GetLocation();
	CurLocation += Pawn->GetActorForwardVector() * 500;
	spawnTransform.SetLocation(CurLocation);
	
	FVector Dir = (Player->GetActorLocation() - Pawn->GetActorLocation()).GetSafeNormal();
	FRotator LookRot = Dir.Rotation();
	spawnTransform.SetRotation(LookRot.Quaternion());


	AVMHomingProjectile* Projectile = GetWorld()->SpawnActor<AVMHomingProjectile>(AVMHomingProjectile::StaticClass(), spawnTransform);*/

	return EBTNodeResult::Succeeded;
}
