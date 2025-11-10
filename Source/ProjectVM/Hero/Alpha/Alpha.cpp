// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/Alpha.h"
#include "Hero/Alpha/EnergyBolt.h"
#include "Hero/VMHeroSkillComponent.h"

AAlpha::AAlpha()
{
	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HairMesh"));
	HairMesh->SetupAttachment(GetMesh());
	HairMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/EchoContent/Characters/Echo/Meshes/Echo.Echo'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> HairMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/EchoContent/Characters/Echo/Meshes/Echo_Hair.Echo_Hair'"));
	if (HairMeshRef.Object)
	{
		HairMesh->SetSkeletalMesh(HairMeshRef.Object);
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

	USkillBase* EnergyBolt = NewObject<UEnergyBolt>(this, UEnergyBolt::StaticClass());
	Skills->BindBasicSkill(EnergyBolt);
}
