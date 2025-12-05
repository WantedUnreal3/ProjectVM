// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/Alpha/VMFireworksEffect.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AVMFireworksEffect::AVMFireworksEffect()
{
	PrimaryActorTick.bCanEverTick = true;
	
	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
    Effect->SetAutoActivate(true);
    RootComponent = Effect;
    
    static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSystemAsset(TEXT("/Game/Fireworks/NiagaraSystem/Ns_Firework_Yellow_Custom.Ns_Firework_Yellow_Custom"));
    if (NiagaraSystemAsset.Succeeded())
    {
    	Effect->SetAsset(NiagaraSystemAsset.Object);
    }
	
	TimeProgress = 0.0f;
}

void AVMFireworksEffect::BeginPlay()
{
	Super::BeginPlay();
}

void AVMFireworksEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeProgress += DeltaTime;
	if (TimeProgress > 10.0f) Destroy();
}

