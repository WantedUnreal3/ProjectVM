// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_SpawnRandomTrap.h"

#include "AOE/RotatingLaserPillar.h"
#include "AOE/VMAOEFloatingBomb.h"

#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

UBTTask_SpawnRandomTrap::UBTTask_SpawnRandomTrap()
{
	bNotifyTaskFinished = true;
}

void UBTTask_SpawnRandomTrap::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UE_LOG(LogTemp, Log, TEXT("[BTTask] SpawnActors 계속 호출?"));
	// CDO가 아니라 인스턴스일 때만 배열을 채움
	if (!HasAnyFlags(RF_ClassDefaultObject))
	{
		if (SpawnActors.Num() == 0)
		{
			SpawnActors.Add(ARotatingLaserPillar::StaticClass());
			SpawnActors.Add(AVMAOEFloatingBomb::StaticClass());

			UE_LOG(LogTemp, Log, TEXT("[BTTask] SpawnActors filled automatically in InitializeFromAsset"));
		}
	}
}

EBTNodeResult::Type UBTTask_SpawnRandomTrap::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (SpawnActors.IsEmpty())
		return EBTNodeResult::Failed;

	SpawnedCount = 0;

	UWorld* World = OwnerComp.GetWorld();
	if (!World) return EBTNodeResult::Failed;

	Index = FMath::RandRange(0, FMath::RandRange(0, SpawnActors.Num() - 1));


	MaxSpawnCount = FMath::RandRange(1, 10);

	// 타이머 시작
	World->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		FTimerDelegate::CreateUObject(
			this,
			&UBTTask_SpawnRandomTrap::SpawnTick,
			&OwnerComp
		),
		SpawnInterval,
		true
	);




	return EBTNodeResult::InProgress;
}

void UBTTask_SpawnRandomTrap::SpawnTick(UBehaviorTreeComponent* OwnerComp)
{
	if (!OwnerComp)
		return;

	AAIController* AIController = OwnerComp->GetAIOwner();
	if (!AIController)
		return;

	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
		return;

	UWorld* World = AIPawn->GetWorld();
	if (!World)
		return;

	// 랜덤 XY
	FVector Origin = AIPawn->GetActorLocation();
	FVector RandomPoint = Origin + FVector(
		FMath::RandRange(-RandomRadius, RandomRadius),
		FMath::RandRange(-RandomRadius, RandomRadius),
		0.f
	);

	// RayCast
	FHitResult Hit;
	FVector Start = RandomPoint + FVector(0, 0, 2000);
	FVector End = RandomPoint + FVector(0, 0, -5000);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(AIPawn);

	bool bHit = World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	if (!bHit) return;  // 이번 틱 스킵

	// Z Offset
	float ZOffset = FMath::RandRange(MinZOffset, MaxZOffset);
	FVector SpawnLoc = Hit.ImpactPoint + FVector(0, 0, ZOffset);

	// 랜덤 클래스
	TSubclassOf<AActor> TrapClass = SpawnActors[Index];

	World->SpawnActor<AActor>(TrapClass, SpawnLoc, FRotator::ZeroRotator);

	SpawnedCount++;

	// 20개 완료 → Task 종료
	if (SpawnedCount >= MaxSpawnCount)
	{
		OwnerComp->GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
		FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
	}
}


void UBTTask_SpawnRandomTrap::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	// Task가 강제로 중지되었을 경우 타이머 정리
	if (OwnerComp.GetWorld())
	{
		OwnerComp.GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}
