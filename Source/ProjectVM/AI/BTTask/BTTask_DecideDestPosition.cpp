// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask/BTTask_DecideDestPosition.h"

#include "AIController.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DecideDestPosition::UBTTask_DecideDestPosition()
{
	NodeName = "Decide Destination Position";
}

EBTNodeResult::Type UBTTask_DecideDestPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AActor* OwningActor = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (OwningActor == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// InitPos 가져오기
	const FVector InitPos = BBComp->GetValueAsVector(TEXT("InitPosition"));

	// X,Y,Z 랜덤 (Z는 최종적으로 지면으로 조정됨)
	FVector RandomPoint;
	RandomPoint.X = InitPos.X + FMath::FRandRange(-SearchRadius, SearchRadius);
	RandomPoint.Y = InitPos.Y + FMath::FRandRange(-SearchRadius, SearchRadius);
	RandomPoint.Z = InitPos.Z + FMath::FRandRange(-SearchRadius, SearchRadius);

	// 1. 아래로 레이 쏴서 바닥 찾기
	FVector StartDown = RandomPoint + FVector(0, 0, 2000.f);
	FVector EndDown = RandomPoint - FVector(0, 0, 10000.f);

	FHitResult HitDown;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(OwningActor);

	bool bHitGround = OwningActor->GetWorld()->LineTraceSingleByChannel(
		HitDown,
		StartDown,
		EndDown,
		ECollisionChannel::ECC_Visibility,
		Params
	);

	if (!bHitGround)
	{
		// 지면을 못 찾았으면 실패
		return EBTNodeResult::Failed;
	}

	FVector GroundPoint = HitDown.ImpactPoint;

	// 2. 위로 레이 쏴서 천장/벽 체크
	FVector StartUp = GroundPoint + FVector(0, 0, GroundOffset);
	FVector EndUp = StartUp + FVector(0, 0, UpperTraceLength);

	FHitResult HitUp;
	bool bHitCeiling = OwningActor->GetWorld()->LineTraceSingleByChannel(
		HitUp,
		StartUp,
		EndUp,
		ECollisionChannel::ECC_Visibility,
		Params
	);

	FVector FinalPos;

	float MinZ = GroundPoint.Z + GroundOffset;   // 바닥에서 최소 500 위
	float MaxZ = GroundPoint.Z + 2*GroundOffset;

	//// 위에 벽(천장)이 있으면 그 바로 아래
	//if (bHitCeiling)
	//{
	//	MaxZ = HitUp.ImpactPoint.Z - GroundOffset;
	//}
	//else
	//{
	//	// 천장 없으면 그냥 StartUp + UpperTraceLength 범위 안에서 Random
	//	MaxZ = StartUp.Z + UpperTraceLength;
	//}

	//// 안전 장치: MinZ > MaxZ면 오류 → 실패처리
	//if (MinZ > MaxZ)
	//{
	//	return EBTNodeResult::Failed;
	//}

	// Z 랜덤선택
	float RandomZ = FMath::FRandRange(MinZ, MaxZ);

	// 최종 도착점
	FinalPos = FVector(RandomPoint.X, RandomPoint.Y, RandomZ);
	UE_LOG(LogTemp, Warning, TEXT("Dest(%f %f %f)"), FinalPos.X, FinalPos.Y, FinalPos.Z);

	// DestPos에 저장
	BBComp->SetValueAsVector(TEXT("DestPosition"), FinalPos);

	return EBTNodeResult::Succeeded;
}