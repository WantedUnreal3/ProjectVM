// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VMAnimNotify_LaserHitCheck.h"

#include "Game/VMPlayer.h"	// Todo: Interface로 변환해야 함.


UVMAnimNotify_LaserHitCheck::UVMAnimNotify_LaserHitCheck()
{
}

void UVMAnimNotify_LaserHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp != nullptr)
	{
		AActor* ActorPtr = MeshComp->GetOwner();
		if (ActorPtr == nullptr)
		{
			return;
		}

		AVMPlayer* PlayerPtr = Cast<AVMPlayer>(ActorPtr);
		if (PlayerPtr == nullptr)
		{
			return;
		}

		PlayerPtr->LaserAttackHitCheck();
	}
}
