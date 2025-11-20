// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SpawnRandomTrap.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTTask_SpawnRandomTrap : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_SpawnRandomTrap();

protected:
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	void SpawnTick(UBehaviorTreeComponent* OwnerComp);

public:
private:
	FTimerHandle SpawnTimerHandle;
	int32 SpawnedCount = 0;
	int32 Index;

public:
	UPROPERTY(EditAnywhere, Category = "Spawn")
	TArray<TSubclassOf<AActor>> SpawnActors;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float RandomRadius = 4000.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MinZOffset = 50.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float MaxZOffset = 300.f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	float SpawnInterval = 0.3f;

	UPROPERTY(EditAnywhere, Category = "Spawn")
	int32 MaxSpawnCount = 20;
};
