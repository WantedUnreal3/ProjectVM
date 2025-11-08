// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "VMAnimNotify_LaserHitCheck.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMAnimNotify_LaserHitCheck : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UVMAnimNotify_LaserHitCheck();

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
