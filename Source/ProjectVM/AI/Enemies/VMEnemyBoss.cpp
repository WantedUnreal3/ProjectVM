// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMEnemyBoss.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"

#include "AI/Enemies/Minions/VMEnemySpawnMelee.h"
#include "AI/Enemies/Minions/VMEnemySpawnRanged.h"
#include "AI/Enemies/Minions/VMEnemySpawnSiege.h"
#include "AI/Enemies/Minions/VMEnemySpawnSuper.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

#include "Macro/VMPhysics.h"

#include "Environment/BossWall.h"
#include "Environment/BossWater.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Enum/BossPhase.h"

#include "Core/VMLevelManager.h"
#include "Portal/VMFieldPortal.h"

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

	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->GravityScale = 0.f;
	GetCharacterMovement()->MaxFlySpeed = 600;

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

	ConstructorHelpers::FObjectFinder<UAnimMontage> MoveMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Animation/AM_BossMove.AM_BossMove'"));
	ensureAlways(MoveMontageRef.Object);
	if (MoveMontageRef.Object)
	{
		MoveMontage = MoveMontageRef.Object;
	}
#pragma endregion

	HPPhase.Push(1000);
	HPPhase.Push(500);
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

	TArray<AActor*> Waters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossWater::StaticClass(), Waters);

	if (Waters.Num() > 0)
	{
		BossWater = Cast<ABossWater>(Waters[0]);
		UE_LOG(LogTemp, Warning, TEXT("BossWall found: %s"), *BossWater->GetName());
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

	//Spawn Level BossMap으로 한정. BossMap 없으면 퍼시스턴트 레벨에 소환
	UVMLevelManager* LevelManager = GetGameInstance()->GetSubsystem<UVMLevelManager>();
	if (LevelManager != nullptr)
	{
		ULevelStreaming* BossLevel = LevelManager->GetLevel(FName("BossMap"));
		if (BossLevel != nullptr && BossLevel->GetLoadedLevel() != nullptr)
		{
			Params.OverrideLevel = BossLevel->GetLoadedLevel();
			UE_LOG(LogTemp, Log, TEXT("Spawn location changed to BossMap"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("BossLevel is nullptr"));
		}
	}
	else 
	{
		UE_LOG(LogTemp, Log, TEXT("LevelManager is nullptr"));
	}
	
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
	OnHealthPointPercentageChanged.Broadcast(CurrentHp / MaxHp);
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

		UWorld* World = GetWorld();
		if (!World) return;

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		//Spawn Level BossMap으로 한정. BossMap 없으면 퍼시스턴트 레벨에 소환
		UVMLevelManager* LevelManager = GetGameInstance()->GetSubsystem<UVMLevelManager>();
		if (LevelManager != nullptr)
		{
			ULevelStreaming* BossLevel = LevelManager->GetLevel(FName("BossMap"));
			if (BossLevel != nullptr && BossLevel->GetLoadedLevel() != nullptr)
			{
				Params.OverrideLevel = BossLevel->GetLoadedLevel();
				UE_LOG(LogTemp, Log, TEXT("Spawn location changed to BossMap"));
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("BossLevel is nullptr"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("LevelManager is nullptr"));
		}

		//필드 포털 생성
		AVMFieldPortal* SpawnedActor = World->SpawnActor<AVMFieldPortal>(AVMFieldPortal::StaticClass(), GetActorTransform(), Params);

		Destroy();
		return;
	}

	if (CurrentHp - HPPhase[PhaseIndex] < KINDA_SMALL_NUMBER)
	{
		PhaseMaxHp = PhaseMinHp;
		PhaseIndex++;

		UpdatePhase();
		if (PhaseIndex == 1)
		{
			BossWall->StartLoweringWall();
			BossWater->StartLoweringWater();
		}
		else if (PhaseIndex == 2)
		{
			BossWall->StartUpperingWall();
		}
		PhaseMinHp = HPPhase[PhaseIndex];
		UE_LOG(LogTemp, Log, TEXT("변경"));
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

void AVMEnemyBoss::UpdatePhase()
{
	int32 NewPhase = 0;

	if (PhaseMaxHp >= 1500)      NewPhase = (int32)EBossPhase::Phase1;
	else if (PhaseMaxHp >= 1000)NewPhase = (int32)EBossPhase::Phase2;
	else                       NewPhase = (int32)EBossPhase::Phase3;
	// Controller 통해 Blackboard에 접근 — Pawn이 직접 소유하지 않음
	AController* Controllers = GetController();
	if (!Controllers) return;

	AAIController* AICon = Cast<AAIController>(Controllers);
	if (!AICon) return;

	UBlackboardComponent* BBComp = AICon->GetBlackboardComponent();
	if (!BBComp) return;

	BBComp->SetValueAsInt(TEXT("BossPhase"), NewPhase);
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