// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VMEnemyBase.h"
#include "VMAIController.h"

#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"

#include "Components/CapsuleComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Engine/DamageEvents.h"

#include "Macro/VMPhysics.h"

#include "Perception/AISense_Hearing.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"

#include "Hero/VMCharacterHeroBase.h"

#include "TimerManager.h"

#include "GameFramework/CharacterMovementComponent.h"



#pragma region 특수_맴버_함수

AVMEnemyBase::AVMEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	//GetCharacterMovement()->bUseAccelerationForPaths = true;
	
	// 시작할 때는 메시를 안보여주도록 설정.
	//GetMesh()->SetHiddenInGame(true);

	// AIControlelr 클래스 설정.
	AIControllerClass = AVMAIController::StaticClass();

	// 맵에서 로드되거나 런타임에 스폰되는 모든 경우에 미리 지정한 AIController에 빙의되도록 설정.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 스켈레탈 메시
	// Todo :기본 매시 사용중 -> 나중에 변경 필요.

	GetCapsuleComponent()->SetCollisionProfileName(VM_ENEMY_COLLISION);
	
#pragma region 스켈레탈메시
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SkeletalMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	ensureAlways(SkeletalMeshRef.Object);
	if (SkeletalMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshRef.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	
	// 애니메이션 입히기
	ConstructorHelpers::FClassFinder<UAnimInstance>AnimInstaceRef(TEXT("/Script/Engine.AnimBlueprint'/Game/Characters/Mannequins/Animations/ABP_Quinn.ABP_Quinn_C'"));
	ensureAlways(AnimInstaceRef.Class);
	if (AnimInstaceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstaceRef.Class);
	}
#pragma endregion

#pragma region Montage
	ConstructorHelpers::FObjectFinder<UAnimMontage> NormalAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_WarriorNormalAttack.AM_WarriorNormalAttack'"));
	if (NormalAttackMontageRef.Object)
	{
		NormalAttackMontage = NormalAttackMontageRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> SkillAttack1MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_WarriorSkill1.AM_WarriorSkill1'"));
	if (SkillAttack1MontageRef.Object)
	{
		SkillAttack1Montage = SkillAttack1MontageRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> SkillAttack2MontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_WarriorSkill2.AM_WarriorSkill2'"));
	if (SkillAttack2MontageRef.Object)
	{
		SkillAttack2Montage = SkillAttack2MontageRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/ArenaBattle/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	ConstructorHelpers::FObjectFinder<UAnimMontage> LaserAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Animation/VM_LaserAttack.VM_LaserAttack'"));
	if (LaserAttackMontageRef.Object)
	{
		LaserAttackMontage = LaserAttackMontageRef.Object;
	}
#pragma endregion

	// Stat
	SetMaxHp(200.0f);
	SetCurrentHp(GetMaxHp());

#pragma region PawnSensing
	EmitterComp = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Emitter"));

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	PawnSensing->SetComponentTickEnabled(true);
	PawnSensing->HearingThreshold = 2000.f;      // 들을 수 있는 기본 거리
	PawnSensing->bOnlySensePlayers = false;
	PawnSensing->LOSHearingThreshold = 2000;		// 시야 내에서 들을 수 있는 거리
	PawnSensing->bEnableSensingUpdates = true;
	PawnSensing->bHearNoises = true;            // 소리 감지 활성화
	PawnSensing->SensingInterval = 0.05f;  // 0.1초마다 감지

	PawnSensing->SightRadius = 1000;
	PawnSensing->SetPeripheralVisionAngle(45);

	PawnSensing->OnSeePawn.AddDynamic(this, &AVMEnemyBase::OnSeePawn);
	PawnSensing->OnHearNoise.AddDynamic(this, &AVMEnemyBase::OnHearPawn);
#pragma endregion

	DeltaTimer = 0.f;
	/*NoiseInterval = 2.f;
	NoiseLoudness = 2500.f;*/

	// Perception Component
	//PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	//SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	//SightConfig->SightRadius = 1500.f;
	//SightConfig->LoseSightRadius = 1600.f;
	//SightConfig->PeripheralVisionAngleDegrees = 60.f;
	//SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//SightConfig->DetectionByAffiliation.bDetectFriendlies = true;


	//HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	//HearingConfig->HearingRange = 2000.f;
	//HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	//HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	//PerceptionComp->ConfigureSense(*HearingConfig);
	//PerceptionComp->SetDominantSense(HearingConfig->GetSenseImplementation());
	//PerceptionComp->ConfigureSense(*SightConfig);
	//PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

	//HearingConfig->DetectionByAffiliation = SightConfig->DetectionByAffiliation;


	//// 감지 이벤트 바인딩
	//PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AVMEnemyBase::OnTargetPerceptionUpdated);

	// AI Perception ----
	//PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));

	//// Sight
	//SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	//SightConfig->SightRadius = 1500.f;
	//SightConfig->LoseSightRadius = 1600.f;
	//SightConfig->PeripheralVisionAngleDegrees = 60.f;
	//SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	//SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	//SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//PerceptionComp->ConfigureSense(*SightConfig);
	//PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

	//// Hearing
	//HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	//HearingConfig->HearingRange = 3000.f;
	//HearingConfig->LoSHearingRange = 3000.f;
	//HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	//HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	//HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	//PerceptionComp->ConfigureSense(*HearingConfig);

	//

	//// 감지 이벤트 바인딩
	//PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AVMEnemyBase::OnTargetPerceptionUpdated);

	EnemyTarget = nullptr;
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

	//DeltaTimer -= DeltaTime;
	//if (DeltaTimer <= 0)
	//{
	//	DeltaTimer += 2;
	//	//EmitterComp->MakeNoise(this, 2500, GetActorLocation());
	//	UAISense_Hearing::ReportNoiseEvent(
	//		GetWorld(),          // World
	//		GetActorLocation(),  // Noise location.
	//		2500,
	//		this,                // Instigator
	//		2500.f,              // Loudness
	//		TEXT("NPCNoise")     // Tag
	//	);
	//	UE_LOG(LogTemp, Log, TEXT("호출되니?"));
	//	DrawDebugSphere(GetWorld(), GetActorLocation(), PawnSensing->HearingThreshold, 16, FColor::Cyan);
	//	if (EnemyTarget == nullptr)
	//	{
	//		UE_LOG(LogTemp, Log, TEXT("NoTarget"));
	//	}
	//	else
	//	{
	//		if (IsValid(EnemyTarget))
	//		{
	//			AAIController* AIController = Cast<AAIController>(GetController());
	//			if (AIController == nullptr)
	//			{
	//				return;
	//			}
	//			UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	//			if (BBComp)
	//			{
	//				BBComp->SetValueAsObject(TEXT("EnemyTarget"), EnemyTarget);
	//			}

	//			UE_LOG(LogTemp, Log, TEXT("[TargetName]:%s"), *EnemyTarget->GetName());
	//		}
	//		else
	//		{
	//			AAIController* AIController = Cast<AAIController>(GetController());
	//			if (AIController == nullptr)
	//			{
	//				return;
	//			}
	//			UBlackboardComponent* BBComp = AIController->GetBlackboardComponent();
	//			if (BBComp)
	//			{
	//				BBComp->SetValueAsObject(TEXT("EnemyTarget"), nullptr);
	//			}
	//		}
	//	}
	//}

}

// Called to bind functionality to input
void AVMEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AVMEnemyBase::GetAIMoveSpeed() const
{
	return 100.0f;
}

float AVMEnemyBase::GetAIAttackRange() const
{
	return 100.0f;
}

float AVMEnemyBase::GetAITurnSpeed() const
{
	return 2.0f;
}

float AVMEnemyBase::GetAIAttackRadius() const
{
	return 100.0f;
}

float AVMEnemyBase::GetAINormalAttackDamage() const
{
	return 10.0f;
}

float AVMEnemyBase::GetAIAttackSpeed() const
{
	return 1.0f;
}

void AVMEnemyBase::NormalAttack()
{
}

void AVMEnemyBase::NormalAttackCheck()
{
	FHitResult OutHitResult;
	TArray<FHitResult> HitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(LaserAttack), false, this);


	float AttackRange_ = GetAIAttackRange();
	float AttackRadius_ = GetAIAttackRadius();
	float AttackDamage_ = GetAINormalAttackDamage();

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange_;

	bool Result = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, VM_HERO_TARGET_ACTION, FCollisionShape::MakeSphere(AttackRadius_), Params);
	if (Result || HitResults.Num())
	{
		for (auto HitResult : HitResults)
		{
			FDamageEvent DamageEvent;
			IVMStatChangeable* IVMStatPtr = Cast<IVMStatChangeable>(HitResult.GetActor());
			if (IVMStatPtr)
			{
				IVMStatPtr->HealthPointChange(AttackRange_, this);
			}
			UE_LOG(LogTemp, Log, TEXT("Name: %s"), *HitResult.GetActor()->GetName());
		}
	}

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange_ * 0.5f;
	FColor Color = Result ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius_, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), Color, false, 5.0f);
}

#pragma endregion

#pragma region IVMAIEnemyBaseInterface

//float AVMEnemyBase::GetAIPatrolRadius()
//{
//	return 500.0f;
//}
//
//float AVMEnemyBase::GetAIDetectRange()
//{
//	return 1500.0f;
//}
//
//float AVMEnemyBase::GetAIAttackRange()
//{
//	return 50.0f;
//}
//
//float AVMEnemyBase::GetAITurnSpeed()
//{
//	return 2.0f;
//}

#pragma endregion

#pragma region IVMStatChangeable 인터페이스 필수 구현 함수

void AVMEnemyBase::HealthPointChange(float Amount, AActor* Causer)
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase::HealthPointChange Damage:%f Causer: %s"), Amount, *Causer->GetName());

	SetCurrentHp(FMath::Clamp<float>(GetCurrentHp() - Amount, 0, GetMaxHp()));

	if (GetCurrentHp() < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Log, TEXT("몬스터가 죽었습니다"));

		Destroy();
	}
}

#pragma endregion

void AVMEnemyBase::LaserAttackHitCheck()
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase::LaserAttackHitCheck"));

	FHitResult OutHitResult;
	TArray<FHitResult> HitResults;
	//TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(LaserAttack), false, this);


	const float AttackRange = 1000.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 30.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	//bool Result = GetWorld()->LineTraceMultiByChannel(OverlapResults, Start, End, ECC_GameTraceChannel1, Params);
	bool Result = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, VM_HERO_TARGET_ACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (Result || HitResults.Num())
	{
		for (auto HitResult : HitResults)
		{
			FDamageEvent DamageEvent;

			IVMStatChangeable* VMStatChangeable = Cast<IVMStatChangeable>(HitResult.GetActor());
			if (VMStatChangeable)
			{
				VMStatChangeable->HealthPointChange(1, this);
				UE_LOG(LogTemp, Log, TEXT("Hited Name: %s"), *HitResult.GetActor()->GetName());
			}
		}
	}

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor Color = Result ? FColor::Green : FColor::Red;
	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), Color, false, 5.0f);
}

#pragma region Pawn Sensing에서 호출될 함수

void AVMEnemyBase::OnSeePawn(APawn* Pawn)
{
	//TryMakeNoise();

	AVMCharacterHeroBase* HeroPawnPtr = Cast<AVMCharacterHeroBase>(Pawn);
	if (HeroPawnPtr == nullptr)
	{
		return;
	}
	//UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase::OnSeePawn : %s"), *Pawn->GetName());
	EnemyTarget = Pawn;

	TryMakeNoise();

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
		{
			BBComp->SetValueAsObject(TEXT("EnemyTarget"), EnemyTarget);
		}
	}
}

void AVMEnemyBase::OnHearPawn(APawn* InstigatorPawn, const FVector& Location, float Volume)
{

	AVMEnemyBase* OtherEnemyBase = Cast<AVMEnemyBase>(InstigatorPawn);
	if (OtherEnemyBase == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No EnemyBase!"));
		return;
	}

	if (OtherEnemyBase->EnemyTarget == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No EnemyBase's Target!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase::OnHearPawn : target %s"), *OtherEnemyBase->EnemyTarget->GetName());
	
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
		{
			BBComp->SetValueAsObject(TEXT("EnemyTarget"), OtherEnemyBase->EnemyTarget);
		}
	}	
}

void AVMEnemyBase::TryMakeNoise()
{
	if (bCanMakeSound == false)
	{
		return;
	}

	bCanMakeSound = false; // 쿨타임 시작

	// 실제 소리 발생
	MakeNoise(1.0f, this, GetActorLocation());

	// 5초 후 다시 소리 가능
	GetWorld()->GetTimerManager().SetTimer(
		SoundTimers,
		this,
		&AVMEnemyBase::ResetSoundCooldown,
		5.0f,
		false
	);
	DrawDebugSphere(GetWorld(), GetActorLocation(), 100.f, 16, FColor::Blue, false, 2.0f);
}

void AVMEnemyBase::ResetSoundCooldown()
{
	bCanMakeSound = true;
}
#pragma endregion

// 감지 이벤트
//void AVMEnemyBase::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
//{
//	if (!Actor)
//		return;
//
//	APawn* SensedPawn = Cast<APawn>(Actor);
//	if (!SensedPawn)
//		return;
//
//	// Sight 감지
//	if (Stimulus.Type == UAISense_Sight::StaticClass()->GetDefaultObject<UAISense_Sight>()->GetSenseID())
//	{
//		AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(SensedPawn);
//		if (Hero == nullptr)
//		{
//			return;
//		}
//		EnemyTarget = SensedPawn;
//		UE_LOG(LogTemp, Log, TEXT("[Sight] Target: %s"), *SensedPawn->GetName());
//
//		// Blackboard 업데이트 (AIController가 있는 경우)
//		if (AAIController* AIController = Cast<AAIController>(GetController()))
//		{
//			if (UBlackboardComponent* BBComp = AIController->GetBlackboardComponent())
//			{
//				BBComp->SetValueAsObject(TEXT("EnemyTarget"), EnemyTarget);
//			}
//		}
//		MakeNoiseForNPC();
//	}
//	// Hearing 감지
//	else if (Stimulus.Type == UAISense_Hearing::StaticClass()->GetDefaultObject<UAISense_Hearing>()->GetSenseID())
//	{
//		// Noise를 발생시킨 Actor를 따라감
//		AVMEnemyBase* EnemyBase = Cast<AVMEnemyBase>(SensedPawn);
//		if (EnemyBase == nullptr)
//		{
//			UE_LOG(LogTemp, Warning, TEXT("Nullptr이야"));
//			return;
//		}
//		if (EnemyBase->EnemyTarget)
//		{
//			UE_LOG(LogTemp, Log, TEXT("[Hearing] Heard Noise from: %s Target:%s"), *SensedPawn->GetName(), *EnemyBase->EnemyTarget->GetName());
//		}
//	}
//
//	
//}
//
//void AVMEnemyBase::MakeNoiseForNPC(float Loudness)
//{
//	if (!GetWorld())
//		return;
//
//	UAISense_Hearing::ReportNoiseEvent(
//		GetWorld(),
//		GetActorLocation(),
//		1.0f,
//		Cast<APawn>(this),
//		3000.f,
//		TEXT("NPCNoise")
//	);
//
//	// 디버그용
//	DrawDebugSphere(GetWorld(), GetActorLocation(), Loudness, 16, FColor::Cyan, false, 1.0f);
//	UE_LOG(LogTemp, Log, TEXT("Noise emitted by: %s"), *GetName());
//}