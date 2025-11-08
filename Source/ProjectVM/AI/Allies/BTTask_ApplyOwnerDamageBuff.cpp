// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/BTTask_ApplyOwnerDamageBuff.h"
#include "BTTask_ApplyOwnerDamageBuff.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Macro/VMAIMarco.h"

#include "BehaviorTree/BehaviorTreeComponent.h"

#include "VMAllyBase.h"
#include "Game/VMPlayer.h"

#include "Particles/ParticleSystemComponent.h"

UBTTask_ApplyOwnerDamageBuff::UBTTask_ApplyOwnerDamageBuff()
{
	bNotifyTick = true; // Tick 사용
}

EBTNodeResult::Type UBTTask_ApplyOwnerDamageBuff::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVMAllyBase* AllyPtr = Cast<AVMAllyBase>(PawnPtr);
	if (AllyPtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// 이벤트 발생으로 하기.
	UAnimInstance* AnimInstancePtr = AllyPtr->GetMesh()->GetAnimInstance();
	if (AnimInstancePtr == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (BBComp == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	AVMPlayer* OwnerPlayer = Cast<AVMPlayer>(BBComp->GetValueAsObject(TEXT("MasterTarget")));
	if (OwnerPlayer == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	// Ally의 스킬을 사용
	AllyPtr->ApplyOwnerDamageBuff();
	AllyPtr->ActivateDamageBuffParticle();

	AnimInstancePtr->Montage_Play(AllyPtr->BuffMontage, 1.0f);

	float UpDamage = 3.0f;
	// 일정 시간 후 원복
	FTimerHandle TimerHandle;
	AllyPtr->GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateUObject(this, &UBTTask_ApplyOwnerDamageBuff::ResetDamage, OwnerPlayer, UpDamage), 60.0f, true);


	return EBTNodeResult::InProgress;
}

void UBTTask_ApplyOwnerDamageBuff::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIControllerPtr = OwnerComp.GetAIOwner();
	if (AIControllerPtr == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* PawnPtr = AIControllerPtr->GetPawn();
	if (PawnPtr == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	AVMAllyBase* AllyPtr = Cast<AVMAllyBase>(PawnPtr);
	if (AllyPtr == nullptr)
	{
		return;
	}

	if (AllyPtr->GetMesh()->GetAnimInstance()->Montage_IsPlaying(AllyPtr->BuffMontage) == true)
	{
		return;
	}

	AllyPtr->DeactivateDamageBuffParticle();
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_ApplyOwnerDamageBuff::ResetDamage(AVMPlayer* Character, float UpDamage)
{
	if (Character == nullptr)
	{
		return;
	}
	Character->AddAttackDamage(-UpDamage);
}
