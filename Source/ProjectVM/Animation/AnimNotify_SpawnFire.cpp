// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_SpawnFire.h"
#include "AnimNotify_SpawnFire.h"

#include "AI/Enemies/VMEnemySpawnBase.h"

UAnimNotify_SpawnFire::UAnimNotify_SpawnFire()
{
}

void UAnimNotify_SpawnFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AVMEnemySpawnBase* EnemySpawnPawn = Cast<AVMEnemySpawnBase>(MeshComp->GetOwner());
	if (EnemySpawnPawn == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[UAnimNotify_SpawnFire::Notify] : EnemySpawnPawn is nullptr"));
		return;
	}

	EnemySpawnPawn->NormalAttack();
	UE_LOG(LogTemp, Log, TEXT("[UAnimNotify_SpawnFire::Notify] : Success"));
}
