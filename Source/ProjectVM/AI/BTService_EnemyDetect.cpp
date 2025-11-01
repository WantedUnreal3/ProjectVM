// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_EnemyDetect.h"

#include "AIController.h"
#include "Interface/VMAIEnemyBaseInterface.h"
#include "Engine/OverlapResult.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Macro/VMAIMarco.h"

#pragma region 특수_맴버_함수

UBTService_EnemyDetect::UBTService_EnemyDetect()
{
}

#pragma endregion

#pragma region 엔진_제공_함수

void UBTService_EnemyDetect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// AIController가 제어하는 폰 가져오기.
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

	// 제어하는 폰이 없으면 함수 종료.
	if (ControllingPawn == nullptr)
	{
		return;
	}

	// 감지 처리를 하는데 필요한 위치 및 월드 가져오기.
	FVector Center = ControllingPawn->GetActorLocation();
	UWorld* World = ControllingPawn->GetWorld();

	// 월드 가져오는데 실패 시 함수 종료.
	if (World == nullptr)
	{
		return;
	}

	// 인터페이스로 형변환.
	IVMAIEnemyBaseInterface* AIPawn = Cast<IVMAIEnemyBaseInterface>(ControllingPawn);

	// 형변환에 실패하면 함수 종료.
	if (AIPawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("여긴듯"));
		return;
	}

	// NPC가 캐릭터를 감지하는 범위 값 가져오기.
	float DetectRadius = AIPawn->GetAIDetectRange();

	// 오버랩을 활용해 캐릭터 충돌 감지.
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParams(SCENE_QUERY_STAT(Detect), false, ControllingPawn);

	// 플레이어가 여러 명이 있다는 가정 하에 Multi로 오버랩 감지.
	bool bResult = World->OverlapMultiByChannel(OverlapResults, Center, FQuat::Identity, ECC_GameTraceChannel1, FCollisionShape::MakeSphere(DetectRadius), CollisionQueryParams);

	UE_LOG(LogTemp, Warning, TEXT("OverlapMultiByChannel returned: %s, Count: %d"), bResult ? TEXT("True") : TEXT("False"), OverlapResults.Num());
	// 충돌이 감지됐으면,
	if (bResult || OverlapResults.Num())
	{
		UE_LOG(LogTemp, Log, TEXT("충돌감지"));
		// 감지된 플레이어에 대해 처리 진행.
		for (auto const& OverlapResult : OverlapResults)
		{
			// 감지된 액터를 폰으로 형변환.
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());

			// 감지된 액터가 폰 타입이고, 폰을 제어하는 컨트롤러가 플레이어 컨트롤러인 경우 처리 진행.
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				// 감지한 캐릭터 정보를 블랙 보드에 저장.
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);

				// 디버그 드로우를 활용해 감지 영역을 녹색으로 표시.
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// 감지된 지점을 녹색으로 표시.
				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Green, false, 0.2f);

				// 감지된 방향을 녹색으로 표시.
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Green, false, 0.27f);
				return;
			}
		}
	}

	// 플레이어를 감지하지 못했다면, 타겟 액터를 null로 저장.
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

	// 감지 영역을 빨간색으로 표시.
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}

#pragma endregion
