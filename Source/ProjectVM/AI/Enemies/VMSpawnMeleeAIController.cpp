// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMSpawnMeleeAIController.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AVMSpawnMeleeAIController::AVMSpawnMeleeAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project/AI/EnemySpawn/BB_VMEnemySpawnMelee.BB_VMEnemySpawnMelee'"));
	ensureAlways(BBAssetRef.Object);

	// Blackboard 에셋 연결
	if (BBAssetRef.Object)
	{
		BBAsset = BBAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project/AI/EnemySpawn/BT_VMEnemySpawnBase.BT_VMEnemySpawnBase'"));
	ensureAlways(BTAssetRef.Object);

	// Behavior Tree 에셋 연결
	if (BTAssetRef.Object)
	{
		BTAsset = BTAssetRef.Object;
		BTAsset->BlackboardAsset = BBAsset;
	}
}
