// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VMEnemyBase.h"
#include "VMAIController.h"


#pragma region 특수_맴버_함수

AVMEnemyBase::AVMEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 시작할 때는 메시를 안보여주도록 설정.
	//GetMesh()->SetHiddenInGame(true);

	// AIControlelr 클래스 설정.
	AIControllerClass = AVMAIController::StaticClass();

	// 맵에서 로드되거나 런타임에 스폰되는 모든 경우에 미리 지정한 AIController에 빙의되도록 설정.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

#pragma endregion

#pragma region 엔진_제공_함수

void AVMEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVMEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVMEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

#pragma endregion

#pragma region IVMAIEnemyBaseInterface

float AVMEnemyBase::GetAIPatrolRadius()
{
	return 500.0f;
}

float AVMEnemyBase::GetAIDetectRange()
{
	return 500.0f;
}

float AVMEnemyBase::GetAIAttackRange()
{
	return 50.0f;
}

float AVMEnemyBase::GetAITurnSpeed()
{
	return 2.0f;
}

#pragma endregion
