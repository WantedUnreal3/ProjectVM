// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VMRPGGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AVMRPGGameMode();

	//모든 액터 BeginPlay 호출 후 실행되는 함수
	void StartPlay() override;

	void GameOver();
	void RestartGame();
};
