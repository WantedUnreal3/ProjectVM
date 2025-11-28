// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMEgg.generated.h"

UCLASS()
class PROJECTVM_API AVMEgg : public AActor
{
	GENERATED_BODY()
	
public:	
	AVMEgg();

protected:

	//보스를 생성하는 함수입니다. BossLevel이 로드되어 있는 상태면 퍼시스턴트 레벨이 아닌 보스레벨에 생성합니다.
	//레벨 로드 관리는 LevelManager에서 합니다.
	UFUNCTION(BlueprintCallable)
	void BossSpawn(FTransform SpawnTransform);
};
