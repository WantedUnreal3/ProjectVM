// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/VMHeroStatComponent.h"

UVMHeroStatComponent::UVMHeroStatComponent()
{
	DefaultStat.AttackPower = 10;
	DefaultStat.DefensivePower = 5;
	DefaultStat.HealthPoint = 100;
	DefaultStat.ManaPoint = 100;
	DefaultStat.ManaRegeneration = 5;
	DefaultStat.Speed = 500;
	DefaultStat.LifeSteal = 10;

	CurStat = DefaultStat;
}

float UVMHeroStatComponent::ApplyDamage(float InDamage)
{
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.0f, InDamage);

	CurStat.HealthPoint = FMath::Clamp<float>(CurStat.HealthPoint - ActualDamage, 0.0f, DefaultStat.HealthPoint);

	if (ActualDamage >= KINDA_SMALL_NUMBER) OnHPChanged.Broadcast(CurStat.HealthPoint);
	if (CurStat.HealthPoint <= KINDA_SMALL_NUMBER) OnDeath.Broadcast();
	
	return ActualDamage;
}

void UVMHeroStatComponent::BeginPlay()
{
	Super::BeginPlay();
}
