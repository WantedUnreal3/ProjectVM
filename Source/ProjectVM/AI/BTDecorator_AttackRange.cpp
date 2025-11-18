// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_AttackRange.h"

#include "AIController.h"
#include "Interface/VMAIEnemyBaseInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Macro/VMAIMarco.h"

#pragma region 특수_맴버_함수

UBTDecorator_AttackRange::UBTDecorator_AttackRange()
{
	NodeName = TEXT("CanAttack");
}

#pragma endregion

#pragma region 엔진_제공_함수

bool UBTDecorator_AttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	// AI 컨트롤러 확인
	if (AIControllerPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("내가 null인가1"));
		return false;
	}

	// AIController가 제어하는 폰 정보 가져오기.
	APawn* PawnPtr = AIControllerPtr->GetPawn();
	// 제어하는 폰이 없는 경우 함수 종료.
	if (PawnPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("내가 null인가2"));
		return false;
	}

	// 인터페이스로 형변환.
	IVMAIEnemyBaseInterface* AIPawnPtr = Cast<IVMAIEnemyBaseInterface>(PawnPtr);
	// 형변환에 실패하면 함수 종료.
	if (AIPawnPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("내가 null인가3"));
		return false;
	}

	// 블랙보드 확인
	UBlackboardComponent* BBComponentPtr = OwnerComp.GetBlackboardComponent();
	if (BBComponentPtr == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("내가 null인가4"));
		return false;
	}

	// 블랙보드에 저장된 캐릭터 정보 가져오기.
	APawn* Target = Cast<APawn>(BBComponentPtr->GetValueAsObject(TEXT("EnemyTarget")));
	// Target이 null이면 함수 종료.
	if (Target == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("내가 null인가5"));
		return false;
	}

	// 캐릭터와의 거리 계산.
	float DistanceToTarget = PawnPtr->GetDistanceTo(Target);

	// 공격 범위 가져오기. // Todo: 하드코딩 지우기
	float AttackRangetWithRadius = AIPawnPtr->GetAIAttackRange();

	// 캐릭터와의 거리와 공격 가능 거리를 비교 후 Result에 저장.
	bResult = (DistanceToTarget <= AttackRangetWithRadius);
	//bResult = BBComponentPtr->GetValueAsBool(TEXT("bCanAttack"));
	//UE_LOG(LogTemp, Display, TEXT("DistanceToTarget:%f AttackRangetWithRadius:%f, flag:%d"), DistanceToTarget, AttackRangetWithRadius, bResult);

	return bResult;
}

#pragma endregion
