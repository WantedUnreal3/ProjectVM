// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SpreadMines.h"

#include "AIController.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PrimitiveComponent.h"

#include "AOE/VMAOEMine.h"



UBTTask_SpreadMines::UBTTask_SpreadMines()
{
	NodeName = TEXT("Spread Mines");
	SpawnCount = 24;
}

void UBTTask_SpreadMines::InitRadialDirections()
{
	if (RadialDirections.Num() > 0) return;

	const int32 Count = 24;
	const float AngleStep = 360.f / Count;

	for (int32 i = 0; i < Count; ++i)
	{
		float AngleRad = FMath::DegreesToRadians(i * AngleStep);
		FVector Dir = FVector(FMath::Cos(AngleRad), FMath::Sin(AngleRad), 0);
		RadialDirections.Add(Dir);
	}
}

EBTNodeResult::Type UBTTask_SpreadMines::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	InitRadialDirections();

	AAIController* AICon = OwnerComp.GetAIOwner();
	if (!AICon) return EBTNodeResult::Failed;

	APawn* AIPawn = AICon->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;

	UWorld* World = AIPawn->GetWorld();
	if (!World) return EBTNodeResult::Failed;

	FVector PawnForward = AIPawn->GetActorForwardVector();
	FVector SpawnBase = AIPawn->GetActorLocation() + PawnForward * SpawnOffset;

	int32 CountToSpawn = FMath::Clamp(SpawnCount, 20, 30);

	for (int32 i = 0; i < CountToSpawn; ++i)
	{
		int32 Index = i % RadialDirections.Num();
		FVector Dir = RadialDirections[Index];

		// Pawn Forward 기준 회전
		FVector SpawnDir = (PawnForward + Dir).GetSafeNormal();

		// 최종 Spawn 위치
		FVector SpawnLoc = SpawnBase + SpawnDir * Radius;
		FRotator SpawnRot = SpawnDir.Rotation();

		AVMAOEMine* SpawnedMine = World->SpawnActor<AVMAOEMine>(AVMAOEMine::StaticClass(), SpawnLoc, SpawnRot);

		if (SpawnedMine)
		{
			// Root Collision 또는 PrimitiveComponent 가져오기
			if (UPrimitiveComponent* Comp = Cast<UPrimitiveComponent>(SpawnedMine->GetComponentByClass(UPrimitiveComponent::StaticClass())))
			{
				Comp->SetSimulatePhysics(true);
				Comp->AddImpulse(SpawnDir * ImpulseStrength, NAME_None, true);
			}
		}
	}

	return EBTNodeResult::Succeeded;
}