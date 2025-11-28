// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Allies/VMAllyBase.h"

#include "AI/Allies/VMAllyAIController.h"

#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"

#include "Perception/PawnSensingComponent.h"
#include "Perception/AIPerceptionComponent.h"

#include "Animation/AnimMontage.h"
#include "Animation/AnimSequence.h"

#include "Components/CapsuleComponent.h"

#include "ProjectVmCharacter.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h" 
#include "NiagaraFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"




// Sets default values
AVMAllyBase::AVMAllyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 시작할 때는 메시를 안보여주도록 설정.
	//GetMesh()->SetHiddenInGame(true);

	// AIControlelr 클래스 설정.
	AIControllerClass = AVMAllyAIController::StaticClass();

	// 맵에서 로드되거나 런타임에 스폰되는 모든 경우에 미리 지정한 AIController에 빙의되도록 설정.
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// 스켈레탈 메시
	// Todo :기본 매시 사용중 -> 나중에 변경 필요.

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AllyPawn"));

#pragma region 스켈레탈메시
	ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonSerath/Characters/Heroes/Serath/Meshes/Serath.Serath'"));
	if (SkeletonMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletonMeshRef.Object);
	}

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	//// 애니메이션 입히기
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimRef(TEXT("/Script/Engine.AnimBlueprint'/Game/ParagonSerath/Characters/Heroes/Serath/Serath_AnimBlueprint.Serath_AnimBlueprint_C'"));
	if (AnimRef.Class)
	{
		GetMesh()->SetAnimClass(AnimRef.Class);
	}
#pragma endregion

#pragma region Montage
	ConstructorHelpers::FObjectFinder<UAnimMontage> HealMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Animation/VM_Heal.VM_Heal'"));
	if (HealMontageRef.Object)
	{
		HealMontage = HealMontageRef.Object;
	}
	
	ConstructorHelpers::FObjectFinder<UAnimMontage> BuffMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/Project/Animation/VM_Buff.VM_Buff'"));
	if (BuffMontageRef.Object)
	{
		BuffMontage = BuffMontageRef.Object;
	}
#pragma endregion

	// 
	HealHpNiagaraSystemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HealHpNiagaraSystemEffect"));

	// 설정
	HealHpNiagaraSystemEffect->SetRelativeLocation(FVector(0, 0, -80));
	HealHpNiagaraSystemEffect->SetupAttachment(RootComponent);
	HealHpNiagaraSystemEffect->bAutoActivate = false; // 자동 재생 방지

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraeHp(TEXT("/Script/Niagara.NiagaraSystem'/Game/KTP_Effect/Particles/Bottom/Bottom04-08.Bottom04-08'"));
	if (NiagaraeHp.Succeeded())
	{
		HealHpNiagaraSystemEffect->SetAsset(NiagaraeHp.Object);
	}

	HealMpNiagaraSystemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("HealMpNiagaraSystemEffect"));

	// 설정
	HealMpNiagaraSystemEffect->SetRelativeLocation(FVector(0, 0, -80));
	HealMpNiagaraSystemEffect->SetupAttachment(RootComponent);
	HealMpNiagaraSystemEffect->bAutoActivate = false; // 자동 재생 방지

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraeMp(TEXT("/Script/Niagara.NiagaraSystem'/Game/KTP_Effect/Particles/Bottom/Bottom02-02.Bottom02-02'"));
	if (NiagaraeMp.Succeeded())
	{
		HealMpNiagaraSystemEffect->SetAsset(NiagaraeMp.Object);
	}


	DamageNiagaraSystemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DamageNiagaraSystemEffect"));

	// 설정
	DamageNiagaraSystemEffect->SetRelativeLocation(FVector(0, 0, -80));
	DamageNiagaraSystemEffect->SetupAttachment(RootComponent);
	DamageNiagaraSystemEffect->bAutoActivate = false; // 자동 재생 방지

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraeDamage(TEXT("/Script/Niagara.NiagaraSystem'/Game/KTP_Effect/Particles/Bottom/Bottom06-05.Bottom06-05'"));
	if (NiagaraeDamage.Succeeded())
	{
		DamageNiagaraSystemEffect->SetAsset(NiagaraeDamage.Object);
	}
}

// Called when the game starts or when spawned AVMAllyBase::BeginPlay()
void AVMAllyBase::BeginPlay()
{
	Super::BeginPlay();
	

	//if (GetOwnerTarget())
	//{
	//	UE_LOG(LogTemp, Log, TEXT("[AVMEnemyBase::BeginPlay] Name:%s"), *GetOwnerTarget()->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("[AVMEnemyBase::BeginPlay] OwnerTarget is nullptr"));
	//}

	//// SetOwnerTarget(GetOwner());

	//// BB 업데이트
	//AVMAIController* AIControllerPtr = Cast<AVMAIController>(GetController());
	//if (AIControllerPtr == nullptr)
	//{
	//	return;
	//}
	//
	//UBlackboardComponent* BBCompPtr = AIControllerPtr->GetBlackboardComponent();
	//if (BBCompPtr == nullptr)
	//{
	//	return;
	//}

	//// 마스터 타겟 설정.
	//BBCompPtr->SetValueAsObject(TEXT("MasterTarget"), GetOwnerTarget());
}

// Called every frame
void AVMAllyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVMAllyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVMAllyBase::HealOwnerHp()
{
	AActor* OwnerActor = GetOwnerTarget();

	if (OwnerActor == nullptr)
	{
		return;
	}

	AVMCharacterHeroBase* OwnerCharacterPtr = Cast<AVMCharacterHeroBase>(OwnerActor);
	if (OwnerCharacterPtr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AVMAllyBase::HealOwnerHp] OwnerCharacterPtr is nullptr"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("주인의 체력을 회복 시도합니다."));
	OwnerCharacterPtr->GetStatComponent()->RecoverHealth(200);
}

void AVMAllyBase::HealOwnerMp()
{
	AActor* OwnerActor = GetOwnerTarget();

	if (OwnerActor == nullptr)
	{
		return;
	}

	AVMCharacterHeroBase* OwnerCharacterPtr = Cast<AVMCharacterHeroBase>(OwnerActor);
	if (OwnerCharacterPtr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AVMAllyBase::HealOwnerMp] OwnerCharacterPtr is nullptr"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("주인의 마나를 회복 시도합니다."));
	OwnerCharacterPtr->GetStatComponent()->RecoverMana(200);
}

void AVMAllyBase::ActivateHpParticle()
{
	HealHpNiagaraSystemEffect->Activate();
}

void AVMAllyBase::DeactivateHpParticle()
{
	//SkillNiagaraSystemEffect->Deactivate();
	HealHpNiagaraSystemEffect->DeactivateImmediate();
}

void AVMAllyBase::ActivateMpParticle()
{
	HealMpNiagaraSystemEffect->Activate();
}

void AVMAllyBase::DeactivateMpParticle()
{
	HealMpNiagaraSystemEffect->Deactivate();
}

void AVMAllyBase::ApplyOwnerDamageBuff()
{
	AActor* OwnerActor = GetOwnerTarget();

	if (OwnerActor == nullptr)
	{
		return;
	}

	// TODO: 인터페이스로 바꿔야 함.

	AVMCharacterHeroBase* OwnerCharacterPtr = Cast<AVMCharacterHeroBase>(OwnerActor);
	if (OwnerCharacterPtr == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[AVMAllyBase::ApplyOwnerDamageBuff] OwnerCharacterPtr is nullptr"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("주인의 데미지를 임시로 올립니다. 윤성님 코드 보고 해야 함."));
}

void AVMAllyBase::ActivateDamageBuffParticle()
{
	DamageNiagaraSystemEffect->Activate();
}

void AVMAllyBase::DeactivateDamageBuffParticle()
{
	DamageNiagaraSystemEffect->Deactivate();
}

AActor* AVMAllyBase::GetOwnerTarget() const
{
	return OwnerTarget;
}

AActor* AVMAllyBase::GetEnemyTarget() const
{
	return EnemyTarget;
}

void AVMAllyBase::SetOwnerTarget(AActor* InOwnerTarget)
{
	OwnerTarget = InOwnerTarget;
	AVMAllyAIController* AIControllerPtr = Cast<AVMAllyAIController>(GetController());
	if (AIControllerPtr)
	{
		UBlackboardComponent* BBCompPtr = AIControllerPtr->GetBlackboardComponent();
		if (BBCompPtr)
		{
			BBCompPtr->SetValueAsObject(TEXT("MasterTarget"), OwnerTarget);
		}
	}
}

void AVMAllyBase::SetEnemyTarget(AActor* InEnemyTarget)
{
	EnemyTarget = InEnemyTarget;
}
