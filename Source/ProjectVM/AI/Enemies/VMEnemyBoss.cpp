// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMEnemyBoss.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"

#include "AI/Enemies/Minions/VMEnemySpawnMelee.h"
#include "AI/Enemies/Minions/VMEnemySpawnRanged.h"
#include "AI/Enemies/Minions/VMEnemySpawnSiege.h"
#include "AI/Enemies/Minions/VMEnemySpawnSuper.h"

#include "Kismet/GameplayStatics.h"

#include "Macro/VMPhysics.h"

#include "Environment/BossWall.h"

// Sets default values
AVMEnemyBoss::AVMEnemyBoss()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	InitDefaultSetting();

	LoadAsset();

	
	// AI 세팅
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	EnemySpawnArray.Push(AVMEnemySpawnMelee::StaticClass());
	EnemySpawnArray.Push(AVMEnemySpawnRanged::StaticClass());
	EnemySpawnArray.Push(AVMEnemySpawnSiege::StaticClass());
	EnemySpawnArray.Push(AVMEnemySpawnSuper::StaticClass());
}

void AVMEnemyBoss::InitDefaultSetting()
{
	// 기본 세팅
	GetCapsuleComponent()->SetCollisionProfileName(VM_ENEMY_COLLISION);
	GetCapsuleComponent()->SetCapsuleHalfHeight(130.0f);
	GetCapsuleComponent()->SetCapsuleRadius(50.0f);
	GetCapsuleComponent()->SetLineThickness(1.0f);
	GetCapsuleComponent()->SetWorldScale3D(FVector(2.0f, 2.0f, 2.0f));

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
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

	HPPhase.Push(2000);
	HPPhase.Push(1000);
	HPPhase.Push(0);
	PhaseIndex = 0;

	CurrentHp = MaxHp;
	PhaseMinHp = HPPhase[PhaseIndex];
}

// Called when the game starts or when spawned
void AVMEnemyBoss::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add("Enemy");

	SaveAllSpawner();

	// Phase2 전용 변수 저장.
	TArray<AActor*> Walls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossWall::StaticClass(), Walls);

	if (Walls.Num() > 0)
	{
		BossWall = Cast<ABossWall>(Walls[0]);
		UE_LOG(LogTemp, Warning, TEXT("BossWall found: %s"), *BossWall->GetName());
	}
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
	int32 Index = FMath::RandRange(0, Spawners.Num() - 1);
	
	FVector SummonLocation = Spawners[Index]->GetActorLocation();
	FTransform SummonTransform(SummonLocation);
	
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	if (EnemySpawnArray.Num() > 0)
	{
		Index = FMath::RandRange(0, EnemySpawnArray.Num() - 1);
		
		AVMEnemySpawnBase* SpawnActor = GetWorld()->SpawnActor<AVMEnemySpawnBase>(EnemySpawnArray[Index], SummonTransform, Params);

		if (SpawnActor == nullptr)
		{
			return;
		}

		UE_LOG(LogTemp, Log, TEXT("소환되었단다. 확인해보렴."));
	}
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

void AVMEnemyBoss::HealthPointChange(float Amount, AActor* Causer)
{
	if (Causer == nullptr)
	{
		return; 
	}

	CurrentHp = FMath::Clamp<float>(CurrentHp - Amount, PhaseMinHp, PhaseMaxHp);
	if (CurrentHp < KINDA_SMALL_NUMBER)
	{
		OnHealthPointPercentageChanged.Broadcast(0);
		Destroy();
		return;
	}

	if (CurrentHp - HPPhase[PhaseIndex] < KINDA_SMALL_NUMBER)
	{
		PhaseMaxHp = PhaseMinHp;
		PhaseIndex++;
		if (PhaseIndex == 1)
		{
			BossWall->StartLoweringWall();
		}
		PhaseMinHp = HPPhase[PhaseIndex];
		UE_LOG(LogTemp, Log, TEXT("변경"));
		//BossWall->WallDown1();
	}
	

	float HpPercent = CurrentHp / MaxHp;
	OnHealthPointPercentageChanged.Broadcast(HpPercent);
}

void AVMEnemyBoss::SaveAllSpawner()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Spawner"), FoundActors);

	Spawners = FoundActors;
	UE_LOG(LogTemp, Log, TEXT("Spawners:%d"), Spawners.Num());
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


void AVMEnemyBoss::ClearDelegate()
{
	OnHealthPointPercentageChanged.RemoveAll(this);
}