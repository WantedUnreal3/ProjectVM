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
	const int ActualDamage = FMath::Clamp<int>(InDamage - CurStats.DefensivePower, 0.0f, InDamage);
 
	CurStats.HealthPoint = FMath::Clamp<int>(CurStats.HealthPoint - ActualDamage, 0.0f, BaseStats.HealthPoint);

	if (ActualDamage > 0) OnHealthPointChanged.Broadcast(CurStats.HealthPoint);
	if (ActualDamage > 0) OnHealthPointPercentageChanged.Broadcast(static_cast<float>(CurStats.HealthPoint) / BaseStats.HealthPoint);
	if (ActualDamage > 0) OnStatChanged.Broadcast(CurStats);
	if (CurStats.HealthPoint <= 0) OnDeath.Broadcast();
}

void UVMHeroStatComponent::RecoverHealth(int32 Amount)
{
	const int ActualRecovery = FMath::Clamp<int>(Amount, 0.0f, Amount);
 
	CurStats.HealthPoint = FMath::Clamp<int>(CurStats.HealthPoint + ActualRecovery, 0.0f, BaseStats.HealthPoint);

	if (ActualRecovery > 0) OnHealthPointChanged.Broadcast(CurStats.HealthPoint);
	if (ActualRecovery > 0) OnHealthPointPercentageChanged.Broadcast(static_cast<float>(CurStats.HealthPoint) / BaseStats.HealthPoint);
	if (ActualRecovery > 0) OnStatChanged.Broadcast(CurStats);
	if (CurStats.HealthPoint <= 0) OnDeath.Broadcast();
}

void UVMHeroStatComponent::ConsumeMana(int32 Amount)
{
	const int32 ActualRecovery = FMath::Clamp<int>(Amount, 0, Amount);
 
	CurStats.ManaPoint = FMath::Clamp<int>(CurStats.ManaPoint - ActualRecovery, 0, BaseStats.ManaPoint);

	if (ActualRecovery > 0) OnManaPointChanged.Broadcast(CurStats.ManaPoint);
	if (ActualRecovery > 0) OnManaPointPercentageChanged.Broadcast(static_cast<float>(CurStats.ManaPoint) / BaseStats.ManaPoint);
	if (ActualRecovery > 0) OnStatChanged.Broadcast(CurStats);
}

void UVMHeroStatComponent::RecoverMana(int32 Amount)
{
	const int32 MaxRecoverable = BaseStats.ManaPoint - CurStats.ManaPoint;
	const int32 ActualAmount = FMath::Clamp<int32>(Amount, 0, MaxRecoverable);
	
	CurStats.ManaPoint = FMath::Clamp<int>(CurStats.ManaPoint + ActualAmount, 0.0f, BaseStats.ManaPoint);

	if (ActualAmount > 0) OnManaPointChanged.Broadcast(CurStats.ManaPoint);
	if (ActualAmount > 0) OnManaPointPercentageChanged.Broadcast(static_cast<float>(CurStats.ManaPoint) / BaseStats.ManaPoint);
	if (ActualAmount > 0) OnStatChanged.Broadcast(CurStats);
}

void UVMHeroStatComponent::ApplyDefaultStat(FHeroStat InStat)
{
	if (BaseStats.HealthPoint != InStat.HealthPoint)
	{
		float HealthPercentage = static_cast<float>(InStat.HealthPoint) / BaseStats.ManaPoint;
		OnHealthPointPercentageChanged.Broadcast(HealthPercentage);
	}
	if (BaseStats.ManaPoint != InStat.ManaPoint)
	{
		float ManaPercentage = static_cast<float>(InStat.ManaPoint) / BaseStats.ManaPoint;
		OnManaPointPercentageChanged.Broadcast(ManaPercentage);
	}

	BaseStats = InStat;
	OnStatChanged.Broadcast(CurStats);
}

void UVMHeroStatComponent::ApplyStat(FHeroStat InStat)
{
	if (CurStats.AttackPower != InStat.AttackPower)
	{
		OnAttackPowerChanged.Broadcast(InStat.AttackPower);
	}
	if (CurStats.DefensivePower != InStat.DefensivePower)
	{
		OnDefensivePowerChanged.Broadcast(InStat.DefensivePower);
	}
	if (CurStats.HealthPoint != InStat.HealthPoint)
	{
		float HealthPercentage = static_cast<float>(InStat.HealthPoint) / BaseStats.ManaPoint;
		OnHealthPointPercentageChanged.Broadcast(HealthPercentage);
		OnHealthPointChanged.Broadcast(InStat.HealthPoint);
	}
	if (CurStats.ManaPoint != InStat.ManaPoint)
	{
		float ManaPercentage = static_cast<float>(InStat.ManaPoint) / BaseStats.ManaPoint;
		OnManaPointPercentageChanged.Broadcast(ManaPercentage);
		OnManaPointChanged.Broadcast(InStat.ManaPoint);
	}
	if (CurStats.ManaRegeneration != InStat.ManaRegeneration)
	{
		OnManaRegenerationChanged.Broadcast(InStat.ManaRegeneration);
	}
	if (CurStats.Speed != InStat.Speed)
	{
		OnSpeedChanged.Broadcast(InStat.Speed);
	}
	if (CurStats.LifeSteal != InStat.LifeSteal)
	{
		OnLifeStealChanged.Broadcast(InStat.LifeSteal);
	}
	
	CurStats = InStat;
	OnStatChanged.Broadcast(CurStats);
}

void UVMHeroStatComponent::ApplyEquipmentStats(UVMEquipment* Equipment)
{
	FVMEquipmentInfo EquipmentInfo = Equipment->GetEquipmentInfo();
	
	FHeroStat NewDefaultStats;

	NewDefaultStats.AttackPower = BaseStats.AttackPower + EquipmentInfo.AttackPower;
	NewDefaultStats.DefensivePower = BaseStats.DefensivePower + EquipmentInfo.DefensivePower;
	NewDefaultStats.HealthPoint = BaseStats.HealthPoint + EquipmentInfo.HealthPoint;
	NewDefaultStats.ManaPoint = BaseStats.ManaPoint + EquipmentInfo.ManaPoint;
	NewDefaultStats.ManaRegeneration = BaseStats.ManaRegeneration + EquipmentInfo.ManaRegeneration;
	NewDefaultStats.LifeSteal = BaseStats.LifeSteal + EquipmentInfo.LifeSteal;

	FHeroStat NewCurStats;

	NewCurStats.AttackPower = CurStats.AttackPower + EquipmentInfo.AttackPower;
	NewCurStats.DefensivePower = CurStats.DefensivePower + EquipmentInfo.DefensivePower;
	NewCurStats.ManaRegeneration = CurStats.ManaRegeneration + EquipmentInfo.ManaRegeneration;
	NewCurStats.LifeSteal = CurStats.LifeSteal + EquipmentInfo.LifeSteal;

	ApplyDefaultStat(NewDefaultStats);
	ApplyStat(NewCurStats);
}

void UVMHeroStatComponent::RemoveEquipmentStats(UVMEquipment* Equipment)
{
	FVMEquipmentInfo EquipmentInfo = Equipment->GetEquipmentInfo();

	FHeroStat NewDefaultStats;

	NewDefaultStats.AttackPower = BaseStats.AttackPower - EquipmentInfo.AttackPower;
	NewDefaultStats.DefensivePower = BaseStats.DefensivePower - EquipmentInfo.DefensivePower;
	NewDefaultStats.HealthPoint = BaseStats.HealthPoint - EquipmentInfo.HealthPoint;
	NewDefaultStats.ManaPoint = BaseStats.ManaPoint - EquipmentInfo.ManaPoint;
	NewDefaultStats.ManaRegeneration = BaseStats.ManaRegeneration - EquipmentInfo.ManaRegeneration;
	NewDefaultStats.LifeSteal = BaseStats.LifeSteal - EquipmentInfo.LifeSteal;

	FHeroStat NewCurStats;

	NewCurStats.AttackPower = CurStats.AttackPower - EquipmentInfo.AttackPower;
	NewCurStats.DefensivePower = CurStats.DefensivePower - EquipmentInfo.DefensivePower;
	NewCurStats.ManaRegeneration = CurStats.ManaRegeneration - EquipmentInfo.ManaRegeneration;
	NewCurStats.LifeSteal = CurStats.LifeSteal - EquipmentInfo.LifeSteal;

	ApplyDefaultStat(NewDefaultStats);
	ApplyStat(NewCurStats);
}

void UVMHeroStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	BaseStats.AttackPower = 10;
	BaseStats.DefensivePower = 5;
	BaseStats.HealthPoint = 100;
	BaseStats.ManaPoint = 100;
	BaseStats.ManaRegeneration = 5;
	BaseStats.Speed = 500;
	BaseStats.LifeSteal = 10;

	CurStats = BaseStats;

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

	FinalStats.AttackPower = FMath::Max(0, BaseStats.AttackPower + AdditiveModifier.AttackPower) * FMath::Max(0, 100 + MultiplicativeModifier.AttackPower);
	FinalStats.DefensivePower = FMath::Max(0, BaseStats.DefensivePower + AdditiveModifier.DefensivePower) * FMath::Max(0, 100 + MultiplicativeModifier.DefensivePower);;
	FinalStats.HealthPoint = FMath::Max(0, BaseStats.HealthPoint + AdditiveModifier.HealthPoint) * FMath::Max(0, 100 + MultiplicativeModifier.HealthPoint);
	FinalStats.ManaPoint = FMath::Max(0, BaseStats.ManaPoint + AdditiveModifier.ManaPoint) * FMath::Max(0, 100 + MultiplicativeModifier.ManaPoint);
	FinalStats.ManaRegeneration = FMath::Max(0, BaseStats.ManaRegeneration + AdditiveModifier.ManaRegeneration) * FMath::Max(0, 100 + MultiplicativeModifier.ManaRegeneration);
	FinalStats.Speed = FMath::Max(0, BaseStats.Speed + AdditiveModifier.Speed) * FMath::Max(0, 100 + MultiplicativeModifier.Speed);
	FinalStats.LifeSteal = FMath::Max(0, BaseStats.LifeSteal + AdditiveModifier.LifeSteal) * FMath::Max(0, 100 + MultiplicativeModifier.LifeSteal);

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
		OnHealthPointChanged.Broadcast(CurStats.HealthPoint);
	}
	if (FinalStats.ManaPoint != PrevFinalStats.ManaPoint)
	{
		float ManaPercentage = static_cast<float>(CurStats.ManaPoint) / FinalStats.ManaPoint;
		OnManaPointPercentageChanged.Broadcast(ManaPercentage);
		OnManaPointChanged.Broadcast(CurStats.ManaPoint);
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
}