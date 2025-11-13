// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/VMAnimNotifyState_LaserHitCheck.h"

#include "Game/VMPlayer.h"
#include "Interface/VMStatChangeable.h"
#include "Interface/LaserAttackInterface.h"
#include "AI/VMEnemyBase.h"
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
		
		//AVMEnemyBase* LaserAttackPawnPtr = Cast<AVMEnemyBase>(ActorPtr);
		//AVMPlayer* LaserAttackPawnPtr = Cast<AVMPlayer>(ActorPtr);
		UE_LOG(LogTemp, Log, TEXT("Owner class: %s"), *ActorPtr->GetClass()->GetName());
		IVMLaserAttackInterface* LaserAttackPawnPtr = Cast<IVMLaserAttackInterface>(ActorPtr);
		if (LaserAttackPawnPtr == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("너구나"));
			return;
		}

		LaserAttackPawnPtr->LaserAttackHitCheck();
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
