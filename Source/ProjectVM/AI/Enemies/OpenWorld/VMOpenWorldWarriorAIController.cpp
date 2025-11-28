// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/OpenWorld/VMOpenWorldWarriorAIController.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Macro/VMAIMarco.h"


AVMOpenWorldWarriorAIController::AVMOpenWorldWarriorAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBWAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project/AI/EnemyOpenWorld/BB_VMWarrior.BB_VMWarrior'"));
	ensureAlways(BBWAssetRef.Object);

	// Blackboard 에셋 연결
	if (BBWAssetRef.Object)
	{
		BBAsset = BBWAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTWAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project/AI/EnemyOpenWorld/BT_VMWarrior.BT_VMWarrior'"));
	ensureAlways(BTWAssetRef.Object);

	// Behavior Tree 에셋 연결
	if (BTWAssetRef.Object)
	{
		BTAsset = BTWAssetRef.Object;
	}
	BTAsset->BlackboardAsset = BBAsset;
}

#pragma region 엔진_제공_함수

void AVMOpenWorldWarriorAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	UE_LOG(LogTemp, Log, TEXT("AVMAIWarriorController::OnPossess"));

	// 컨트롤러가 폰에 빙의하면 AI를 실행하도록 함수 실행.
	RunAI();
}

void AVMOpenWorldWarriorAIController::OnUnPossess()
{
	UE_LOG(LogTemp, Log, TEXT("AVMAIWarriorController::OnUnPossess"));
}

void AVMOpenWorldWarriorAIController::RunAI()
{
	// 블랙보드 컴포넌트 포인터 가져오기.
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	UE_LOG(LogTemp, Log, TEXT("AVMAIWarriorController::RunAI()"));

	// 사용할 블랙보드 지정.
	if (UseBlackboard(BBAsset, BlackboardPtr))
	{
		UE_LOG(LogTemp, Log, TEXT("Here!!!"));
		// 시작 시 좌표를 Blackboard의 HomePos에 저장.
		BlackboardPtr->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());

		// 블랙보드 설정에 잘 진행됐으면, 행동트리 실행.
		bool RunResult = RunBehaviorTree(BTAsset);

		// 실행에 실패하면 오류가 발생하도록 지정.
		ensureAlways(RunResult);
	}
}

void AVMOpenWorldWarriorAIController::StopAI()
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
