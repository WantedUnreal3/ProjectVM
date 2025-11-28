// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/OpenWorld/VMEnemyOpenWorldWarrior.h"

#include "AI/Enemies/OpenWorld/VMOpenWorldWarriorAIController.h"
#include "Components/CapsuleComponent.h"

#include "Macro/VMPhysics.h"

#include "Item/VMItemCube.h"
#include "Quest/VMQuestManager.h"

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

void AVMEnemyOpenWorldWarrior::HealthPointChange(float Amount, AActor* Causer)
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase::HealthPointChange Damage:%f Causer: %s"), Amount, *Causer->GetName());

	SetCurrentHp(FMath::Clamp<float>(GetCurrentHp() - Amount, 0, GetMaxHp()));

	if (GetCurrentHp() < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Log, TEXT("몬스터가 죽었습니다"));
		TSubclassOf<AVMItemCube> ActorToSpawn = AVMItemCube::StaticClass();
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		AVMItemCube* SpawnedActor = GetWorld()->SpawnActor<AVMItemCube>(ActorToSpawn, SpawnLocation, SpawnRotation);
		GetGameInstance()->GetSubsystem<UVMQuestManager>()->NotifyMonsterDeath(MonsterName);
		Destroy();
	}
}
