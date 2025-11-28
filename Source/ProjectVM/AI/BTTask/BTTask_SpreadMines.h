// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpreadMines.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_SpreadMines : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SpreadMines();

	/** 폭탄/마인 클래스 */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<AActor>> ProjectileClass;

	/** 반경 */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float Radius = 200.0f;

	/** Impulse 세기 */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	float ImpulseStrength = 1200.f;

	/** 스폰 개수 (20~30) */
	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 SpawnCount = 24;

	UPROPERTY(EditAnywhere, Category = "Mines")
	float SpawnOffset = 100.f; // Pawn 앞에서 시작

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	/** 24개의 단위 방향 벡터 */
	TArray<FVector> RadialDirections;

	/** 최초 1회 방향 계산 */
	void InitRadialDirections();

	bool bHasSpawned = false;
};
