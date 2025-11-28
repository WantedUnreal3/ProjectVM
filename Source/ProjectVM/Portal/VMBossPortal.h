// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal/VMPortal.h"
#include "VMBossPortal.generated.h"

UCLASS()
class PROJECTVM_API AVMBossPortal : public AVMPortal
{
	GENERATED_BODY()

public:
	AVMBossPortal();
protected:
	virtual void Interact() override;
	void LoadMap();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetMap")
	FString TargetLevelPath = "/Game/Project/Map/RealBossMap.RealBossMap";
	FName TargetLevelName = FName("BossMap"); //레벨 매니저에 저장할 키 이름
};
