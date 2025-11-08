// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTService_CheckOwnerDistance.h"

#include "AIController.h"
#include "VMAllyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_CheckOwnerDistance::UBTService_CheckOwnerDistance()
{
	NodeName = TEXT("Master와 거리 체크");
}

void UBTService_CheckOwnerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// AI 컨트롤러 획득 시도
	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return ;
	}

	// 폰 획득 시도
	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		return ;
	}

	// 블랙 보드 얻기 시도.
	UBlackboardComponent* BBComponentPtr = OwnerComp.GetBlackboardComponent();
	if (BBComponentPtr == nullptr)
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("CheckOwnerDistance::TickNode"));

	// 진짜 작업 시작.
	APawn* MasterPtr = Cast<APawn>(BBComponentPtr->GetValueAsObject(TEXT("MasterTarget")));
	if (MasterPtr == nullptr)
	{
		return;
	}
	FVector MasterLocation = MasterPtr->GetActorLocation();
	FVector MyLocation = PawnPtr->GetActorLocation();
	float Distance = FVector::Distance(MasterLocation, MyLocation);
	BBComponentPtr->SetValueAsFloat(TEXT("Distance"), Distance);
}

void UBTService_CheckOwnerDistance::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	//UE_LOG(LogTemp, Log, TEXT("CheckOwnerDistance::OnSearchStart"));
}

