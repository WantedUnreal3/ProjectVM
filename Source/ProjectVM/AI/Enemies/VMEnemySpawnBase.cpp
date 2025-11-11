// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMEnemySpawnBase.h"

// Sets default values
AVMEnemySpawnBase::AVMEnemySpawnBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	InitDefaultAssetSetting();
}


// Called when the game starts or when spawned
void AVMEnemySpawnBase::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add("Enemy");
}

// Called every frame
void AVMEnemySpawnBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVMEnemySpawnBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVMEnemySpawnBase::OnHealHp(float HealGauge)
{
	CurrentHp = FMath::Clamp<float>(CurrentHp + HealGauge, 0, MaxHp);
}

void AVMEnemySpawnBase::InitDefaultAssetSetting()
{
	// Skeletal Mesh 입히기
#pragma region SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonMinions/Characters/Buff/Buff_Blue/Meshes/Buff_Blue.Buff_Blue'"));
	ensureAlways(MeshRef.Object);
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
#pragma endregion 
}

