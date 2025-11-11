// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/MyVMSpawnMeleeAIController.h"

#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyVMSpawnMeleeAIController::AMyVMSpawnMeleeAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBWAssetRef(TEXT("/Script/AIModule.BlackboardData'/Game/Project/AI/EnemySpawn/BB_VMEnemySpawnMelee.BB_VMEnemySpawnMelee'"));
	ensureAlways(BBWAssetRef.Object);

	// Blackboard 에셋 연결
	if (BBWAssetRef.Object)
	{
		BBAsset = BBWAssetRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTWAssetRef(TEXT("/Script/AIModule.BehaviorTree'/Game/Project/AI/EnemySpawn/BT_VMEnemySpawnBase.BT_VMEnemySpawnBase'"));
	ensureAlways(BTWAssetRef.Object);

	// Behavior Tree 에셋 연결
	if (BTWAssetRef.Object)
	{
		BTAsset = BTWAssetRef.Object;
		BTAsset->BlackboardAsset = BBAsset;
	}
}
