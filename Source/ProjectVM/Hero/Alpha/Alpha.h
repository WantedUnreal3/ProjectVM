// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Alpha.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AAlpha : public AVMCharacterHeroBase
{
	GENERATED_BODY()

public:
	AAlpha();
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkeletalMeshComponent> HairMesh;
};
