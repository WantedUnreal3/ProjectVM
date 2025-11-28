// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_DecideDestPosition.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_DecideDestPosition : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_DecideDestPosition();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	/** InitPos BB Key */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector InitPosKey;

	/** DestPos BB Key */
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	struct FBlackboardKeySelector DestPosKey;

	/** 랜덤 탐색 반경 */
	UPROPERTY(EditAnywhere, Category = "Search")
	float SearchRadius = 3000.f;

	/** 지면에서 띄울 거리 */
	UPROPERTY(EditAnywhere, Category = "Search")
	float GroundOffset = 400.f;

	/** 윗 레이 벽 체크용 길이 */
	UPROPERTY(EditAnywhere, Category = "Search")
	float UpperTraceLength = 500.f;
};
