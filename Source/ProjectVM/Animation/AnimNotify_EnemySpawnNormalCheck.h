// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_EnemySpawnNormalCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UAnimNotify_EnemySpawnNormalCheck : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_EnemySpawnNormalCheck();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
