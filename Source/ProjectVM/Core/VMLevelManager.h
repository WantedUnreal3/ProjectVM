// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VMLevelManager.generated.h"

UCLASS()
class PROJECTVM_API UVMLevelManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<class ULevelStreaming> GetLevel(FName LevelName)
	{
		TObjectPtr<ULevelStreaming>* FoundLevel = LoadedLevels.Find(LevelName);
		if (FoundLevel == nullptr)
		{
			return nullptr;
		}
		return *FoundLevel;
	}

	void AddLevel(FName NewLevelName, TObjectPtr<class ULevelStreaming> NewLevel);

	void DeleteLevel(FName Level);
private:
	TMap < FName, TObjectPtr<class ULevelStreaming>> LoadedLevels;
};
