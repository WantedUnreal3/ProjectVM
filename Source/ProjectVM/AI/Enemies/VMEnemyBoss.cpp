// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMEnemyBoss.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"

#include "AI/Enemies/VMEnemySpawnMelee.h"

// Sets default values
AVMEnemyBoss::AVMEnemyBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	InitDefaultSetting();

	LoadAsset();

	
	// AI 세팅
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AVMEnemyBoss::InitDefaultSetting()
{
	// 기본 세팅
	GetCapsuleComponent()->SetCapsuleHalfHeight(130.0f);
	GetCapsuleComponent()->SetCapsuleRadius(50.0f);
	GetCapsuleComponent()->SetLineThickness(1.0f);
	GetCapsuleComponent()->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));


	GetMesh()->SetRelativeLocation(FVector(0, 0, -120.0f));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}

void AVMEnemyBoss::LoadAsset()
{
	// Skeletal Mesh 입히기
#pragma region SkeletalMesh
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Skins/Tier_1/Sevarog_Red/Meshes/SevarogBloodred.SevarogBloodred'"));
	ensureAlways(MeshRef.Object);
	if (MeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(MeshRef.Object);
	}
#pragma endregion

#pragma region AnimationClass
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Sevarog_AnimBlueprint.Sevarog_AnimBlueprint_C'"));
	ensureAlways(AnimInstanceRef.Class);
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimClass(AnimInstanceRef.Class);
	}
#pragma endregion

#pragma region AIControllerClass
	ConstructorHelpers::FClassFinder<AController> AIControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProjectVM.VMBossAIController'"));
	ensureAlways(AIControllerClassRef.Class);
	if (AIControllerClassRef.Class)
	{
		AIControllerClass = AIControllerClassRef.Class;
	}
#pragma endregion

#pragma region MontageObject
	ConstructorHelpers::FObjectFinder<UAnimMontage> SummonMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ParagonSevarog/Characters/Heroes/Sevarog/Animations/AM_SummonMinion.AM_SummonMinion'"));
	ensureAlways(SummonMontageRef.Object);
	if (SummonMontageRef.Object)
	{
		SummonMontage = SummonMontageRef.Object;
	}
#pragma endregion
}

void AVMEnemyBoss::ActivateSummonMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}

	const float SummonMontageRate = 1.0f;
	AnimInstance->Montage_Play(SummonMontage, SummonMontageRate);

}

void AVMEnemyBoss::SummonMinion(FVector Pos)
{
	//FVector CurretLocation = GetActorLocation();
	FVector SummonLocation = Pos;
	FTransform SummonTransform(SummonLocation);
	AVMEnemySpawnMelee* SpawnActor = GetWorld()->SpawnActor<AVMEnemySpawnMelee>(AVMEnemySpawnMelee::StaticClass(), SummonTransform);
	if (SpawnActor == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("소환되었단다. 확인해보렴."));
}

void AVMEnemyBoss::DeactivateSummonMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance == nullptr)
	{
		return;
	}

	const float SummonMontageRate = 1.0f;
	AnimInstance->Montage_Stop(0.0f, SummonMontage);
}

void AVMEnemyBoss::OnHealHp(float HealGauge)
{
	CurrentHp = FMath::Clamp<float>(CurrentHp + HealGauge, 0, MaxHp);
}

// Called when the game starts or when spawned
void AVMEnemyBoss::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add("Enemy");
}

// Called every frame
void AVMEnemyBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVMEnemyBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



// Wrapper 함수 느낌.
//void AVMEnemyBoss::SummonMinions()
//{
//	ActivateSummonMinion();
//}
//
//float AVMEnemyBoss::GetSummonRange() const
//{
//	return 500.0f;
//}
//
//int32 AVMEnemyBoss::GetNumOfSummon() const
//{
//	return FMath::RandRange(0, 2);
//}
//void AVMEnemyBoss::ActivateSummonMinion()
//{
//	for (int32 i = 0; i < GetNumOfSummon(); ++i)
//	{
//		UE_LOG(LogTemp, Log, TEXT("소환 몬스터"));
//	}
//}
