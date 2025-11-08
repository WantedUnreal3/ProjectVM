// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VMAnimNotifyState_LaserHitCheck.h"

#include "Game/VMPlayer.h"

void UVMAnimNotifyState_LaserHitCheck::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	if (AVMPlayer* Player = Cast<AVMPlayer>(MeshComp->GetOwner()))
	{
		//Player->SetLaserVisible(true);
	}
}

void UVMAnimNotifyState_LaserHitCheck::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

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

void UVMAnimNotifyState_LaserHitCheck::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (AVMPlayer* Player = Cast<AVMPlayer>(MeshComp->GetOwner()))
	{
		//Player->SetLaserVisible(false);
	}
}
