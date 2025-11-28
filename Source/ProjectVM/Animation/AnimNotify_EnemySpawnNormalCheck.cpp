// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_EnemySpawnNormalCheck.h"
#include "AnimNotify_EnemySpawnNormalCheck.h"

#include "Interface/VMAIEnemyBaseInterface.h"

UAnimNotify_EnemySpawnNormalCheck::UAnimNotify_EnemySpawnNormalCheck()
{

}

void UAnimNotify_EnemySpawnNormalCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	IVMAIEnemyBaseInterface* IEnemyBasePtr = Cast<IVMAIEnemyBaseInterface>(MeshComp->GetOwner());
	if (IEnemyBasePtr == nullptr)
	{
		return;
	}

	// 노말 공격 범위만큼 체크
	IEnemyBasePtr->NormalAttackCheck();
}
