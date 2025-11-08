// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_UpdateHp.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UBTService_UpdateHp : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_UpdateHp();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
};
