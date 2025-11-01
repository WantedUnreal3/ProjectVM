// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VMAIController.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Macro/VMAIMarco.h"

#pragma region 특수_맴버_함수

AVMAIController::AVMAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project/AI/BB_VMEnemyBase.BB_VMEnemyBase'"));
	ensureAlways(BBAssetRef.Object);

	// Blackboard 에셋 연결
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project/AI/BT_VMEnemyBase.BT_VMEnemyBase'"));
	ensureAlways(BTAssetRef.Object);

	// Behavior Tree 에셋 연결
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
	}
}

#pragma endregion

#pragma region 엔진_제공_함수

void AVMAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("OnPossess"));

	// 컨트롤러가 폰에 빙의하면 AI를 실행하도록 함수 실행.
	RunAI();
}

void AVMAIController::OnUnPossess()
{
	UE_LOG(LogTemp, Log, TEXT("OnUnPossess"));
}

#pragma endregion

#pragma region 메시지_함수

void AVMAIController::RunAI()
{
	// 블랙보드 컴포넌트 포인터 가져오기.
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	// 사용할 블랙보드 지정.
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		// 시작 시 좌표를 Blackboard의 HomePos에 저장.
		BlackboardPtr->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 블랙보드 설정에 잘 진행됐으면, 행동트리 실행.
		bool RunResult = RunBehaviorTree(BTAsset);

		// 실행에 실패하면 오류가 발생하도록 지정.
		ensureAlways(RunResult);
	}
}

void AVMAIController::StopAI()
{
	// 실행 중인 BehaviorTreeComponent 가져오기.
	UBehaviorTreeComponent* BTComponentPtr = Cast<UBehaviorTreeComponent>(BrainComponent);

	// BT 실행 중지.
	if (BTComponentPtr)
	{
		BTComponentPtr->StopTree();
	}
}

#pragma endregion
