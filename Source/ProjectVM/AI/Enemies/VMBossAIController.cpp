// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMBossAIController.h"

#include "Interface/VMStatChangeable.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Game/VMGameMode.h"

#include "Hero/VMCharacterHeroBase.h"

#include "Kismet/GameplayStatics.h"

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

	ACharacter* PlayerCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AVMBossAIController::RunAI] 월드에서 찾은 PlayerCharacter가 nullptr입니다."));
		return;
	}
	
	// 위를 통과했다면 무조건 존재.
	BlackboardPtr->SetValueAsObject(TEXT("EnemyTarget"), PlayerCharacter);

	bool RunResult = RunBehaviorTree(BTAsset);
	if (RunResult == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("[AVMBossAIController::RunAI] RunBehaviorTree를 실패하였습니다."));
		return;
	}
}

void AVMBossAIController::BeginPlay()
{
	Super::BeginPlay();

	// Todo: 삭제해야 함. BossMap에서 테스트하기 위해 넣은 코드
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
