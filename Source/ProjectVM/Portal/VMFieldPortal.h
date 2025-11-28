// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Portal/VMPortal.h"
#include "VMFieldPortal.generated.h"

UCLASS()
class PROJECTVM_API AVMFieldPortal : public AVMPortal
{
	GENERATED_BODY()

public:
	AVMFieldPortal();

protected:
	void Interact() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DeleteLevelName = FName("BossMap");
};
