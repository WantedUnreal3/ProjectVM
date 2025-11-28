// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMFlamingLaunchEffect.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AVMFlamingLaunchEffect::AVMFlamingLaunchEffect()
{
 	PrimaryActorTick.bCanEverTick = true;

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetAutoActivate(true);
	Effect->SetAutoDestroy(true);
	RootComponent = Effect;

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Game/PortalSystem/FX/NS_SpeedDashLines.NS_SpeedDashLines"));
	if (NiagaraSystemAsset.Succeeded())
	{
		Effect->SetAsset(NiagaraSystemAsset.Object);
	}

	TimeProgress = 0.0f;
}

void AVMFlamingLaunchEffect::BeginPlay()
{
	Super::BeginPlay();
}

void AVMFlamingLaunchEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeProgress += DeltaTime;
	if (TimeProgress > 1.0f) Destroy();
}

