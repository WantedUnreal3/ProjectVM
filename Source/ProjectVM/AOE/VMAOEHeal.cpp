// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/VMAOEHeal.h"

#include "Components/SphereComponent.h"
#include "AI/Enemies/VMEnemySpawnBase.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

#include "Macro/VMPhysics.h"

// Sets default values
AVMAOEHeal::AVMAOEHeal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 생성 및 루트 지정
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereComp;

	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComp->SetCollisionObjectType(VM_ENEMY_TARGET_ACTION);
	SphereComp->InitSphereRadius(300.f);

	// 이벤트 등록.
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AVMAOEHeal::OnSphereBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &AVMAOEHeal::OnSphereEndOverlap);

	AOEEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("AOEEffect"));
	AOEEffect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_HealEffect(
		TEXT("/Script/Engine.ParticleSystem'/Game/FXVarietyPack/Particles/P_ky_healAura.P_ky_healAura'")
	);
	if (PS_HealEffect.Succeeded())
	{
		AOEEffect->SetTemplate(PS_HealEffect.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundBase> SB_HealSound(
		TEXT("/Script/Engine.SoundCue'/Game/StarterContent/Audio/Explosion_Cue.Explosion_Cue'")
	);
	if (SB_HealSound.Succeeded())
	{
		AOESound = SB_HealSound.Object;
	}
}

// Called when the game starts or when spawned
void AVMAOEHeal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVMAOEHeal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TotalTime += DeltaTime;
	if (TotalTime >= DeltaTick)
	{
		TotalTime -= DeltaTick;

		// Heal하면 될 듯
		ProcessHeal();
	}

	MaxTime -= DeltaTime;
	if (MaxTime <= 0)
	{
		Destroy();
	}
}

void AVMAOEHeal::ProcessHeal()
{
	for (auto Actor : InnerActorArray)
	{
		IEnemyHealInterface* EnemySpawnBase = Cast<IEnemyHealInterface>(Actor);
		if (EnemySpawnBase == nullptr)
		{
			continue;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("에이?"));
		EnemySpawnBase->OnHealHp(HpGauge);
	}
}

void AVMAOEHeal::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("이름 들어옴.: %s"), *OtherActor->GetName());
	InnerActorArray.Add(OtherActor);
}

void AVMAOEHeal::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("이름 나감: %s"), *OtherActor->GetName());
	InnerActorArray.Remove(OtherActor);
}
