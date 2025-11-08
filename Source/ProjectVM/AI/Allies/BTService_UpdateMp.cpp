// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTService_UpdateMp.h"

#include "AIController.h"
#include "VMAllyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/VMPlayer.h"

UBTService_UpdateMp::UBTService_UpdateMp()
{
	NodeName = TEXT("Update Owner's Mp");
}

void UBTService_UpdateMp::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UE_LOG(LogTemp, Log, TEXT("UpdateMp::TickNode"));

	// AI 컨트롤러 획득 시도
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return;
	}

	// 폰 획득 시도
	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		return;
	}

	// 블랙 보드 얻기 시도.
	UBlackboardComponent* BBComponentPtr = OwnerComp.GetBlackboardComponent();
	if (BBComponentPtr == nullptr)
	{
		return;
	}

	// 진짜 작업 시작.
	APawn* MasterPtr = Cast<APawn>(BBComponentPtr->GetValueAsObject(TEXT("MasterTarget")));
	if (MasterPtr == nullptr)
	{
		return;
	}

	AVMPlayer* OwnerPlayerPtr = Cast<AVMPlayer>(MasterPtr);
	if (MasterPtr == nullptr)
	{
		return;
	}

	float CurrentMp = OwnerPlayerPtr->GetCurrentMp();
	float MaxMp = OwnerPlayerPtr->GetMaxMp();

	BBComponentPtr->SetValueAsFloat(TEXT("OwnerCurrentMp"), CurrentMp);
	BBComponentPtr->SetValueAsFloat(TEXT("OwnerMaxMp"), MaxMp);
}

void UBTService_UpdateMp::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	UE_LOG(LogTemp, Log, TEXT("UpdateMp::OnMpStart"));
}
