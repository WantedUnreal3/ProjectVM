// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_CheckHitFlag.h"

#include "Components/SkeletalMeshComponent.h"
#include "AI/VMEnemyBase.h"

UAnimNotify_CheckHitFlag::UAnimNotify_CheckHitFlag()
{
}

void UAnimNotify_CheckHitFlag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	AVMEnemyBase* EnemyBase = Cast<AVMEnemyBase>(MeshComp->GetOwner());
	if (EnemyBase == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[AnimNotify_CheckHitFlag] EnemyBase nullptr"));
		return;
	}

	UAnimInstance* AnimInstance = EnemyBase->GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[AnimNotify_CheckHitFlag] AnimInstance nullptr"));
		return;
	}

	if (EnemyBase->CurrentSectionIndex == 4)
	{
		EnemyBase->CurrentSectionIndex = 1;
	}
	else
	{
		//FString NextSection = FString::Printf(TEXT("%s%d"), *EnemyBase->SectionNamePrefix, EnemyBase->CurrentSectionIndex);
		FString NextSection = EnemyBase->SectionNamePrefix + FString::FromInt(EnemyBase->CurrentSectionIndex);
		FName SectionName(*NextSection);
		AnimInstance->Montage_JumpToSection(SectionName, EnemyBase->NormalAttackMontage);

		EnemyBase->CurrentSectionIndex = EnemyBase->CurrentSectionIndex + 1;
	}

	
}
