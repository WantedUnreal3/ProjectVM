// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTService_UpdateHp.h"

#include "AIController.h"
#include "VMAllyBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Game/VMPlayer.h"

#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"

UBTService_UpdateHp::UBTService_UpdateHp()
{
	NodeName = TEXT("Update Owner's Hp");
}

void UBTService_UpdateHp::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

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

	
	//AVMPlayer* OwnerPlayerPtr = Cast<AVMPlayer>(MasterPtr);
	AVMCharacterHeroBase* OwnerPlayerPtr = Cast<AVMCharacterHeroBase>(MasterPtr);
	if (MasterPtr == nullptr)
	{
		return;
	}

	float CurrentHp = OwnerPlayerPtr->GetStatComponent()->GetStat().HealthPoint;
	float MaxHp = OwnerPlayerPtr->GetStatComponent()->GetDefaultStat().HealthPoint;
	UE_LOG(LogTemp, Log, TEXT("Cur:%f, Max:%f"), CurrentHp, MaxHp);
	BBComponentPtr->SetValueAsFloat(TEXT("OwnerCurrentHp"), CurrentHp);
	BBComponentPtr->SetValueAsFloat(TEXT("OwnerMaxHp"), MaxHp);
}

void UBTService_UpdateHp::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);
}
