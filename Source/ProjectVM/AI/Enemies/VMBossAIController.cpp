// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMBossAIController.h"
#include "VMBossAIController.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/VMPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Game/VMGameMode.h"

AVMBossAIController::AVMBossAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBWAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project/AI/EnemyBoss/BB_VMBoss.BB_VMBoss'"));
	ensureAlways(BBWAssetRef.Object);

	// Blackboard 에셋 연결
	if (BBWAssetRef.Object)
	{
		BBAsset = BBWAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTWAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project/AI/EnemyBoss/BT_VMBoss.BT_VMBoss'"));
	ensureAlways(BTWAssetRef.Object);

	// Behavior Tree 에셋 연결
	if (BTWAssetRef.Object)
	{
		BTAsset = BTWAssetRef.Object;
		BTAsset->BlackboardAsset = BBAsset;
	}
}

void AVMBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();
}

void AVMBossAIController::OnUnPossess()
{
	Super::OnUnPossess();

	StopAI();
}

void AVMBossAIController::RunAI()
{
	UBlackboardComponent* BlackboardPtr = Blackboard.Get();

	bool UseResult = UseBlackboard(BBAsset, BlackboardPtr);
	if (UseResult == false)
	{
		ensureAlways(UseResult);
		return;
	}

	APawn* PawnPtr = GetPawn();

	BlackboardPtr->SetValueAsVector(TEXT("InitPosition"), PawnPtr->GetActorLocation());

	AVMPlayer* PlayerCharacter = Cast<AVMPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("저는 NULLPtr"));
	}

	if (PlayerCharacter)
	{
		// 블랙보드에 저장 (키 이름은 Blackboard Asset에서 설정한 것과 동일해야 함)
		BlackboardPtr->SetValueAsObject(TEXT("EnemyTarget"), PlayerCharacter);
		APawn* QWER = Cast<APawn>(BlackboardPtr->GetValueAsObject(TEXT("EnemyTarget")));
		if (QWER == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Set EnemyTarget result Error")); // true면 성공
		}
		
		
	}

	bool RunResult = RunBehaviorTree(BTAsset);
	if (RunResult == false)
	{
		ensureAlways(RunResult);
		return;
	}
}
void AVMBossAIController::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		UBlackboardComponent* BBComp = GetBlackboardComponent();
		if (BBComp)
		{
			BBComp->SetValueAsObject(TEXT("EnemyTarget"), PlayerCharacter);
		}
	}

	RunBehaviorTree(BTAsset);
}

void AVMBossAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//
	//UBlackboardComponent* BBComp = GetBlackboardComponent();
	//if (BBComp)
	//{
	//	if (BBComp->GetValueAsObject(TEXT("EnemyTarget")))
	//	{
	//		return;
	//	}

	//	AVMPlayer* PlayerCharacter = Cast<AVMPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//	if (PlayerCharacter)
	//	{
	//		BBComp->SetValueAsObject(TEXT("EnemyTarget"), PlayerCharacter);
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("Player Character is null"));
	//	}
	//}
}

// AI를 중지하는 함수.
void AVMBossAIController::StopAI()
{
	// 실행 중인 BehaviorTreeComponent 가져오기.
	UBehaviorTreeComponent* BTComponentPtr = Cast<UBehaviorTreeComponent>(BrainComponent);

	// BT 실행 중지.
	if (BTComponentPtr == nullptr)
	{
		return;
	}

	BTComponentPtr->StopTree();
}
