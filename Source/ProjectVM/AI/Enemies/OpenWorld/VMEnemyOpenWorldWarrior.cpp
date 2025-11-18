// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/OpenWorld/VMEnemyOpenWorldWarrior.h"

#include "AI/Enemies/OpenWorld/VMOpenWorldWarriorAIController.h"
#include "Components/CapsuleComponent.h"

#include "Macro/VMPhysics.h"

AVMEnemyOpenWorldWarrior::AVMEnemyOpenWorldWarrior()
{
#pragma region AI_OpenWorldWarriorAIController
	AIControllerClass = AVMOpenWorldWarriorAIController::StaticClass();
#pragma endregion

#pragma region CapsuleComponent
	// 콜리전 Enemy 전용으로 변경
	GetCapsuleComponent()->SetCollisionProfileName(VM_ENEMY_COLLISION);
#pragma endregion

#pragma region Skeletal_Mesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonAurora/Characters/Heroes/Aurora/Meshes/Aurora.Aurora'"));
	ensureAlways(SkeletalMeshRef.Object);
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	// 콜리전 NoCollision으로 변경
	GetMesh()->SetCollisionProfileName(NOCOLLISION);
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	// 애니메이션 입히기
	ConstructorHelpers::FClassFinder<UAnimInstance>AnimInstaceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonAurora/Characters/Heroes/Aurora/Aurora_AnimBlueprint.Aurora_AnimBlueprint_C'"));
	ensureAlways(AnimInstaceRef.Class);
	if (AnimInstaceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstaceRef.Class);
	}
#pragma endregion

#pragma region Montage
	ConstructorHelpers::FObjectFinder<UAnimMontage> NormalAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Animation/AM_AuroraAttack.AM_AuroraAttack'"));
	if (NormalAttackMontageRef.Object)
	{
		NormalAttackMontage = NormalAttackMontageRef.Object;
	}
#pragma endregion
}
