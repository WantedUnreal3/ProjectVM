// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/VMHeroStatComponent.h"

UVMHeroStatComponent::UVMHeroStatComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UVMHeroStatComponent::ApplyDamage(int32 InDamage)
{
	const int ActualDamage = FMath::Clamp<int>(InDamage - CurStat.DefensivePower, 0.0f, InDamage);
 
	CurStat.HealthPoint = FMath::Clamp<int>(CurStat.HealthPoint - ActualDamage, 0.0f, DefaultStat.HealthPoint);

	if (ActualDamage > 0) OnHealthPointChanged.Broadcast(CurStat.HealthPoint);
	if (ActualDamage > 0) OnHealthPointPercentageChanged.Broadcast(static_cast<float>(CurStat.HealthPoint) / DefaultStat.HealthPoint);
	if (ActualDamage > 0) OnStatChanged.Broadcast(CurStat);
	if (CurStat.HealthPoint <= 0) OnDeath.Broadcast();
}

void UVMHeroStatComponent::RecoverHealth(int32 Amount)
{
	const int ActualRecovery = FMath::Clamp<int>(Amount, 0.0f, Amount);
 
	CurStat.HealthPoint = FMath::Clamp<int>(CurStat.HealthPoint + ActualRecovery, 0.0f, DefaultStat.HealthPoint);

	if (ActualRecovery > 0) OnHealthPointChanged.Broadcast(CurStat.HealthPoint);
	if (ActualRecovery > 0) OnHealthPointPercentageChanged.Broadcast(static_cast<float>(CurStat.HealthPoint) / DefaultStat.HealthPoint);
	if (ActualRecovery > 0) OnStatChanged.Broadcast(CurStat);
	if (CurStat.HealthPoint <= 0) OnDeath.Broadcast();
}

void UVMHeroStatComponent::ConsumeMana(int32 Amount)
{
	const int32 ActualRecovery = FMath::Clamp<int>(Amount, 0, Amount);
 
	CurStat.ManaPoint = FMath::Clamp<int>(CurStat.ManaPoint - ActualRecovery, 0, DefaultStat.ManaPoint);

	if (ActualRecovery > 0) OnManaPointChanged.Broadcast(CurStat.ManaPoint);
	if (ActualRecovery > 0) OnManaPointPercentageChanged.Broadcast(static_cast<float>(CurStat.ManaPoint) / DefaultStat.ManaPoint);
	if (ActualRecovery > 0) OnStatChanged.Broadcast(CurStat);
}

void UVMHeroStatComponent::RecoverMana(int32 Amount)
{
	const int32 MaxRecoverable = DefaultStat.ManaPoint - CurStat.ManaPoint;
	const int32 ActualAmount = FMath::Clamp<int32>(Amount, 0, MaxRecoverable);
	
	CurStat.ManaPoint = FMath::Clamp<int>(CurStat.ManaPoint + ActualAmount, 0.0f, DefaultStat.ManaPoint);

	if (ActualAmount > 0) OnManaPointChanged.Broadcast(CurStat.ManaPoint);
	if (ActualAmount > 0) OnManaPointPercentageChanged.Broadcast(static_cast<float>(CurStat.ManaPoint) / DefaultStat.ManaPoint);
	if (ActualAmount > 0) OnStatChanged.Broadcast(CurStat);
}

void UVMHeroStatComponent::ApplyStat(FHeroStat InStat)
{
	if (CurStat.AttackPower != InStat.AttackPower)
	{
		OnAttackPowerChanged.Broadcast(InStat.AttackPower);
	}
	if (CurStat.DefensivePower != InStat.DefensivePower)
	{
		OnDefensivePowerChanged.Broadcast(InStat.DefensivePower);
	}
	if (CurStat.HealthPoint != InStat.HealthPoint)
	{
		float HealthPercentage = static_cast<float>(InStat.HealthPoint) / DefaultStat.ManaPoint;
		OnHealthPointPercentageChanged.Broadcast(HealthPercentage);
		OnHealthPointChanged.Broadcast(InStat.HealthPoint);
	}
	if (CurStat.ManaPoint != InStat.ManaPoint)
	{
		float ManaPercentage = static_cast<float>(InStat.ManaPoint) / DefaultStat.ManaPoint;
		OnManaPointPercentageChanged.Broadcast(ManaPercentage);
		OnManaPointChanged.Broadcast(InStat.ManaPoint);
	}
	if (CurStat.ManaRegeneration != InStat.ManaRegeneration)
	{
		OnManaRegenerationChanged.Broadcast(InStat.ManaRegeneration);
	}
	if (CurStat.Speed != InStat.Speed)
	{
		OnSpeedChanged.Broadcast(InStat.Speed);
	}
	if (CurStat.LifeSteal != InStat.LifeSteal)
	{
		OnLifeStealChanged.Broadcast(InStat.LifeSteal);
	}
	
	CurStat = InStat;
	OnStatChanged.Broadcast(CurStat);
}

void UVMHeroStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	DefaultStat.AttackPower = 10;
	DefaultStat.DefensivePower = 5;
	DefaultStat.HealthPoint = 100;
	DefaultStat.ManaPoint = 100;
	DefaultStat.ManaRegeneration = 5;
	DefaultStat.Speed = 500;
	DefaultStat.LifeSteal = 10;

	CurStat = DefaultStat;

	TimeProgress = 0.0f;
}

void UVMHeroStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeProgress += DeltaTime;
	if (TimeProgress >= 1.0f)
	{
		RecoverMana(CurStat.ManaRegeneration);
		TimeProgress -= 1.0f;
	}
}
