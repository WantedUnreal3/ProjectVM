// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CheckHitFlag.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UAnimNotify_CheckHitFlag : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAnimNotify_CheckHitFlag();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
