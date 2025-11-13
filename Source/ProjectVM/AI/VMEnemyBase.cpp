// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/VMEnemyBase.h"
#include "VMAIController.h"

#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"

#include "Components/CapsuleComponent.h"

#include "Engine/DamageEvents.h"

#include "Macro/VMPhysics.h"



#pragma region 특수_맴버_함수

AVMEnemyBase::AVMEnemyBase()
{
	//PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bCanEverTick = true;
	
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

	// PawnSensing
	//PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	//PawnSensing->SetComponentTickEnabled(true);
	//PawnSensing->HearingThreshold = 2000.f;      // 들을 수 있는 기본 거리
	//PawnSensing->LOSHearingThreshold = 2000;		// 시야 내에서 들을 수 있는 거리
	//PawnSensing->bEnableSensingUpdates = true;
	//PawnSensing->bHearNoises = true;            // 소리 감지 활성화
	//PawnSensing->SensingInterval = 0.05f;  // 0.1초마다 감지

	//PawnSensing->OnSeePawn.AddDynamic(this, &AVMEnemyBase::OnSeePawn);
	//PawnSensing->OnHearNoise.AddDynamic(this, &AVMEnemyBase::OnHearPawn);

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

	// DrawDebugSphere(GetWorld(), GetActorLocation(), PawnSensing->HearingThreshold, 16, FColor::Cyan);
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
	return 1500.0f;
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

#pragma region IVMStatChangeable 인터페이스 필수 구현 함수

void AVMEnemyBase::HealthPointChange(float Amount, AActor* Causer)
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase::HealthPointChange Damage:%f Causer: %s"), Amount, *Causer->GetName());

	SetCurrentHp(FMath::Clamp<float>(GetCurrentHp() - Amount, 0, GetMaxHp()));

	if (GetCurrentHp() < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Log, TEXT("몬스터가 죽었습니다"));
	}
}

#pragma endregion

#pragma region Pawn Sensing에서 호출될 함수

//void AVMEnemyBase::OnSeePawn(APawn* Pawn)
//{
//	//UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase::OnSeePawn : %s"), *Pawn->GetName());
//}
//
//void AVMEnemyBase::OnHearPawn(APawn* InstigatorPawn, const FVector& Location, float Volume)
//{
//	UE_LOG(LogTemp, Log, TEXT("AVMEnemyBase::OnHearPawn : %s"), *InstigatorPawn->GetName());
//	/*if (!InstigatorPawn)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Heard noise but InstigatorPawn is null!"));
//		return;
//	}
//
//	UE_LOG(LogTemp, Warning, TEXT("Heard noise from %s at %s, volume %.2f"),
//		*InstigatorPawn->GetName(),
//		*Location.ToString(),
//		Volume);*/
//
//	DrawDebugSphere(GetWorld(), Location, 100.f, 16, FColor::Blue, false, 2.0f);
//}

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
