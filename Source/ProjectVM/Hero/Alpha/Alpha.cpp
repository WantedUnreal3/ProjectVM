// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/Alpha.h"
#include "Hero/Alpha/VMSparksFly.h"
#include "Hero/Alpha/VMBurningFuse.h"
#include "Hero/Alpha/VMFlamingLaunch.h"
#include "Hero/Alpha/VMFireworks.h"
#include "Hero/VMHeroSkillComponent.h"
#include "Hero/VMHeroStatComponent.h"

AAlpha::AAlpha()
{
	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh());
	HairMesh->SetCollisionProfileName(TEXT("VMHeroCollision"));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/EchoContent/Characters/Echo/Meshes/Echo.Echo'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HairMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/EchoContent/Characters/Echo/Meshes/Echo_Hair.Echo_Hair'"));
	if (HairMeshRef.Object)
	{
		HairMesh->SetSkeletalMesh(HairMeshRef.Object);
		HairMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(TEXT("/Game/Project/Hero/Alpha/ABP_Alpha.ABP_Alpha_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}
}

void AAlpha::BeginPlay()
{
	Super::BeginPlay();

	USkillBase* SparksFly = NewObject<UVMSparksFly>(this, UVMSparksFly::StaticClass());
	USkillBase* BurningFuse = NewObject<UVMBurningFuse>(this, UVMBurningFuse::StaticClass());
	USkillBase* FlamingLaunch = NewObject<UVMFlamingLaunch>(this, UVMFlamingLaunch::StaticClass());
	USkillBase* Fireworks = NewObject<UVMFireworks>(this, UVMFireworks::StaticClass());

	FHeroStat BaseStats;
	BaseStats.AttackPower = 10;
	BaseStats.DefensivePower = 5;
	BaseStats.HealthPoint = 100;
	BaseStats.ManaPoint = 100;
	BaseStats.ManaRegeneration = 5;
	BaseStats.Speed = 500;
	BaseStats.LifeSteal = 10;
	
	Stat->InitBaseStats(BaseStats);

	Skills->BindBasicSkill(SparksFly);
	Skills->BindAdvancedSkill(BurningFuse);
	Skills->BindMovementSkill(FlamingLaunch);
	Skills->BindUltimateSkill(Fireworks);
}
