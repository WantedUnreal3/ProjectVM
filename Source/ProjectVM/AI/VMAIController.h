// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "VMAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMAIController : public AAIController
{
	GENERATED_BODY()

#pragma region 특수_맴버_함수
public:
	AVMAIController();
#pragma endregion

#pragma region 엔진_제공_함수
protected:
	// 컨트롤러가 폰에 빙의했을 때 실행되는 함수.
	virtual void OnPossess(APawn* InPawn) override;

	// 컨트롤러가 폰에 빙의해제했을 때 실행되는 함수.
	virtual void OnUnPossess() override;
#pragma endregion

#pragma region 메시지_함수
public:
	// AI를 실행하는 함수.
	virtual void RunAI();

	// AI를 중지하는 함수.
	virtual void StopAI();
#pragma endregion

#pragma region 맴버_변수
protected:
	// Blackboard 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Asset)
	TObjectPtr<class UBlackboardData> BBAsset;

	// Behaviour Tree 애셋.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Asset)
	TObjectPtr<class UBehaviorTree> BTAsset;
#pragma endregion
};
