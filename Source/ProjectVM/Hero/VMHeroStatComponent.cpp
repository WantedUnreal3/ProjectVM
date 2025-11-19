// Fill out your copyright notice in the Description page of Project Settings.


#include "Hero/VMHeroStatComponent.h"
#include "Item/Equipment/VMEquipment.h"

UVMHeroStatComponent::UVMHeroStatComponent()
{
	bWantsInitializeComponent = true;
	PrimaryComponentTick.bCanEverTick = true;

	TimeProgress = 0.0f;
}

void UVMHeroStatComponent::ApplyDamage(int32 InDamage)
{
	const int32 ActualDamage = FMath::Clamp<int32>(InDamage - CurStats.DefensivePower, 0, InDamage);
 
	CurStats.HealthPoint = FMath::Clamp<int32>(CurStats.HealthPoint - ActualDamage, 0, FinalStats.HealthPoint);

	if (ActualDamage > 0) OnCurrentHealthPointChanged.Broadcast(CurStats.HealthPoint);
	if (ActualDamage > 0) OnHealthPointPercentageChanged.Broadcast(static_cast<float>(CurStats.HealthPoint) / FinalStats.HealthPoint);
	if (CurStats.HealthPoint <= 0) OnDeath.Broadcast();
}

void UVMHeroStatComponent::RecoverHealth(int32 Amount)
{
	const int32 ActualRecovery = FMath::Max<int32>(0, Amount);
 
	CurStats.HealthPoint = FMath::Clamp<int32>(CurStats.HealthPoint + ActualRecovery, 0.0f, FinalStats.HealthPoint);

	if (ActualRecovery > 0) OnCurrentHealthPointChanged.Broadcast(CurStats.HealthPoint);
	if (ActualRecovery > 0) OnHealthPointPercentageChanged.Broadcast(static_cast<float>(CurStats.HealthPoint) / FinalStats.HealthPoint);
	if (CurStats.HealthPoint <= 0) OnDeath.Broadcast();
}

void UVMHeroStatComponent::ConsumeMana(int32 Amount)
{
	const int32 ActualRecovery = FMath::Max<int32>(0, Amount);
 
	CurStats.ManaPoint = FMath::Clamp<int32>(CurStats.ManaPoint - ActualRecovery, 0, FinalStats.ManaPoint);

	if (ActualRecovery > 0) OnCurrentManaPointChanged.Broadcast(CurStats.ManaPoint);
	if (ActualRecovery > 0) OnManaPointPercentageChanged.Broadcast(static_cast<float>(CurStats.ManaPoint) / BaseStats.ManaPoint);
}

void UVMHeroStatComponent::RecoverMana(int32 Amount)
{
	const int32 ActualAmount = FMath::Max<int32>(0, Amount);
	
	CurStats.ManaPoint = FMath::Clamp<int32>(CurStats.ManaPoint + ActualAmount, 0.0f, FinalStats.ManaPoint);

	if (ActualAmount > 0) OnCurrentManaPointChanged.Broadcast(CurStats.ManaPoint);
	if (ActualAmount > 0) OnManaPointPercentageChanged.Broadcast(static_cast<float>(CurStats.ManaPoint) / BaseStats.ManaPoint);
}

void UVMHeroStatComponent::ApplyEquipmentStats(UVMEquipment* Equipment)
{
	FVMEquipmentInfo EquipmentInfo = Equipment->GetEquipmentInfo();
	
	EquipmentStats.AttackPower += EquipmentInfo.AttackPower;
	EquipmentStats.DefensivePower += EquipmentInfo.DefensivePower;
	EquipmentStats.HealthPoint += EquipmentInfo.HealthPoint;
	EquipmentStats.ManaPoint += EquipmentInfo.ManaPoint;
	EquipmentStats.ManaRegeneration += EquipmentInfo.ManaRegeneration;
	EquipmentStats.Speed += EquipmentInfo.Speed;
	EquipmentStats.LifeSteal += EquipmentInfo.LifeSteal;

	CalcFinalStats();
}

void UVMHeroStatComponent::RemoveEquipmentStats(UVMEquipment* Equipment)
{
	FVMEquipmentInfo EquipmentInfo = Equipment->GetEquipmentInfo();

	EquipmentStats.AttackPower -= EquipmentInfo.AttackPower;
	EquipmentStats.DefensivePower -= EquipmentInfo.DefensivePower;
	EquipmentStats.HealthPoint -= EquipmentInfo.HealthPoint;
	EquipmentStats.ManaPoint -= EquipmentInfo.ManaPoint;
	EquipmentStats.ManaRegeneration -= EquipmentInfo.ManaRegeneration;
	EquipmentStats.Speed -= EquipmentInfo.Speed;
	EquipmentStats.LifeSteal -= EquipmentInfo.LifeSteal;

	CalcFinalStats();
}

void UVMHeroStatComponent::ApplyAdditiveModifier(FHeroStat InStat)
{
	AdditiveModifier.AttackPower += InStat.AttackPower;
	AdditiveModifier.DefensivePower += InStat.DefensivePower;
	AdditiveModifier.HealthPoint += InStat.HealthPoint;
	AdditiveModifier.ManaPoint += InStat.ManaPoint;
	AdditiveModifier.ManaRegeneration += InStat.ManaRegeneration;
	AdditiveModifier.Speed += InStat.Speed;
	AdditiveModifier.LifeSteal += InStat.LifeSteal;

	CalcFinalStats();
}

void UVMHeroStatComponent::ApplyMultiplicativeModifier(FHeroStat InStat)
{
	MultiplicativeModifier.AttackPower += InStat.AttackPower;
	MultiplicativeModifier.DefensivePower += InStat.DefensivePower;
	MultiplicativeModifier.HealthPoint += InStat.HealthPoint;
	MultiplicativeModifier.ManaPoint += InStat.ManaPoint;
	MultiplicativeModifier.ManaRegeneration += InStat.ManaRegeneration;
	MultiplicativeModifier.Speed += InStat.Speed;
	MultiplicativeModifier.LifeSteal += InStat.LifeSteal;

	CalcFinalStats();
}

void UVMHeroStatComponent::InitBaseStats(FHeroStat InStats)
{
	BaseStats = InStats;

	CalcFinalStats();
	RecoverHealth(BaseStats.HealthPoint);
	RecoverMana(BaseStats.ManaPoint);
}

void UVMHeroStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	TimeProgress = 0.0f;
}

void UVMHeroStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeProgress += DeltaTime;
	if (TimeProgress >= 1.0f)
	{
		RecoverMana(CurStats.ManaRegeneration);
		TimeProgress -= 1.0f;
	}
}

void UVMHeroStatComponent::CalcFinalStats()
{
	FHeroStat PrevFinalStats = FinalStats;

	FinalStats.AttackPower = FMath::Max(0, BaseStats.AttackPower + EquipmentStats.AttackPower + AdditiveModifier.AttackPower) * FMath::Max(0, 100 + MultiplicativeModifier.AttackPower) / 100;
	FinalStats.DefensivePower = FMath::Max(0, BaseStats.DefensivePower + EquipmentStats.DefensivePower + AdditiveModifier.DefensivePower) * FMath::Max(0, 100 + MultiplicativeModifier.DefensivePower) / 100;
	FinalStats.HealthPoint = FMath::Max(0, BaseStats.HealthPoint + EquipmentStats.HealthPoint + AdditiveModifier.HealthPoint) * FMath::Max(0, 100 + MultiplicativeModifier.HealthPoint) / 100;
	FinalStats.ManaPoint = FMath::Max(0, BaseStats.ManaPoint + EquipmentStats.ManaPoint + AdditiveModifier.ManaPoint) * FMath::Max(0, 100 + MultiplicativeModifier.ManaPoint) / 100;
	FinalStats.ManaRegeneration = FMath::Max(0, BaseStats.ManaRegeneration + EquipmentStats.ManaRegeneration + AdditiveModifier.ManaRegeneration) * FMath::Max(0, 100 + MultiplicativeModifier.ManaRegeneration) / 100;
	FinalStats.Speed = FMath::Max(0, BaseStats.Speed + EquipmentStats.Speed + AdditiveModifier.Speed) * FMath::Max(0, 100 + MultiplicativeModifier.Speed) / 100;
	FinalStats.LifeSteal = FMath::Max(0, BaseStats.LifeSteal + EquipmentStats.LifeSteal + AdditiveModifier.LifeSteal) * FMath::Max(0, 100 + MultiplicativeModifier.LifeSteal) / 100;
	
	if (FinalStats.AttackPower != PrevFinalStats.AttackPower)
	{
		OnAttackPowerChanged.Broadcast(FinalStats.AttackPower);
	}
	if (FinalStats.DefensivePower != PrevFinalStats.DefensivePower)
	{
		OnDefensivePowerChanged.Broadcast(FinalStats.DefensivePower);
	}
	if (FinalStats.HealthPoint != PrevFinalStats.HealthPoint)
	{
		float HealthPercentage = static_cast<float>(CurStats.HealthPoint) / FinalStats.HealthPoint;
		OnHealthPointPercentageChanged.Broadcast(HealthPercentage);
		OnHealthPointChanged.Broadcast(FinalStats.HealthPoint);
	}
	if (FinalStats.ManaPoint != PrevFinalStats.ManaPoint)
	{
		float ManaPercentage = static_cast<float>(CurStats.ManaPoint) / FinalStats.ManaPoint;
		OnManaPointPercentageChanged.Broadcast(ManaPercentage);
		OnManaPointChanged.Broadcast(FinalStats.ManaPoint);
	}
	if (FinalStats.ManaRegeneration != PrevFinalStats.ManaRegeneration)
	{
		OnManaRegenerationChanged.Broadcast(FinalStats.ManaRegeneration);
	}
	if (FinalStats.Speed != PrevFinalStats.Speed)
	{
		OnSpeedChanged.Broadcast(FinalStats.Speed);
	}
	if (FinalStats.LifeSteal != PrevFinalStats.LifeSteal)
	{
		OnLifeStealChanged.Broadcast(FinalStats.LifeSteal);
	}

	OnStatChanged.Broadcast(FinalStats);

	CurStats.AttackPower = FinalStats.AttackPower;
	CurStats.DefensivePower = FinalStats.DefensivePower;
	CurStats.ManaRegeneration = FinalStats.ManaRegeneration;
	CurStats.Speed = FinalStats.Speed;
	CurStats.LifeSteal = FinalStats.LifeSteal;
}