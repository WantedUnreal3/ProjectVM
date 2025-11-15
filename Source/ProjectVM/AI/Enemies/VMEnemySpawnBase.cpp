// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemies/VMEnemySpawnBase.h"

#include "Components/CapsuleComponent.h"
#include "Quest/VMQuestManager.h"


#include "Engine/DamageEvents.h"

#include "Interface/VMStatChangeable.h"

#include "Macro/VMPhysics.h"

// Sets default values
AVMEnemySpawnBase::AVMEnemySpawnBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName(VM_ENEMY_COLLISION);

	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetCollisionProfileName(NOCOLLISION);

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

void AVMEnemySpawnBase::HealthPointChange(float Amount, AActor* Causer)
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemySpawnBase::HealthPointChange Damage:%f Causer: %s"), Amount, *Causer->GetName());

	SetCurrentHp(FMath::Clamp<float>(GetCurrentHp() - Amount, 0, GetMaxHp()));

	if (GetCurrentHp() < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Log, TEXT("몬스터가 죽었습니다"));
		GetGameInstance()->GetSubsystem<UVMQuestManager>()->NotifyMonsterDeath(GetMonsterType());
		Destroy();
	}
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

void AVMEnemySpawnBase::NormalAttack()
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemySpawnBase::NormalAttack %s"), *GetName());
	PlayNormalAttackMontage();
}

void AVMEnemySpawnBase::NormalAttackCheck()
{
	UE_LOG(LogTemp, Log, TEXT("AVMEnemySpawnBase::NormalAttackCheck"));

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

void AVMEnemySpawnBase::PlayNormalAttackMontage()
{
	// 공격 애니메이션을 실행한다.
	UAnimInstance* AnimInstancePtr = GetMesh()->GetAnimInstance();
	if (AnimInstancePtr == nullptr)
	{
		return;
	}

	const float AnimPlayRate = GetAIAttackSpeed();
	AnimInstancePtr->Montage_Play(NormalAttackMontage, 1.0f);
	
	UE_LOG(LogTemp, Log, TEXT("PlayNormalAttackMontage 재생"));
}

