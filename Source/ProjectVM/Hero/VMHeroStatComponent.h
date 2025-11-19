// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/HeroStat.h"
#include "Components/ActorComponent.h"
#include "VMHeroStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FStatHandler, FHeroStat /* FinalStat */);
DECLARE_MULTICAST_DELEGATE_OneParam(FAttackPowerHandler, int32 /* FinalAtk */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDefensivePowerHandler, int32 /* FinalDef */);
DECLARE_MULTICAST_DELEGATE_OneParam(FHealthPointHandler, int32 /* FinalHP */);
DECLARE_MULTICAST_DELEGATE_OneParam(FManaPointHandler, int32 /* FinalMP */);
DECLARE_MULTICAST_DELEGATE_OneParam(FManaRegenerationHandler, int32 /* FinalMPRegen */);
DECLARE_MULTICAST_DELEGATE_OneParam(FSpeedHandler, int32 /* FinalSpeed */);
DECLARE_MULTICAST_DELEGATE_OneParam(FLifeStealHandler, int32 /* FinalSpeed */);

DECLARE_MULTICAST_DELEGATE_OneParam(FCurrentHealthPointHandler, float /* CurrentHP */);
DECLARE_MULTICAST_DELEGATE_OneParam(FCurrentManaPointHandler, float /* CurrentMP */);
DECLARE_MULTICAST_DELEGATE_OneParam(FHealthPointPercentageHandler, float /* HPPercentage */);
DECLARE_MULTICAST_DELEGATE_OneParam(FManaPointPercentageHandler, float /* MPPercentage */);
DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVM_API UVMHeroStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVMHeroStatComponent();

	FStatHandler OnStatChanged;
	FAttackPowerHandler OnAttackPowerChanged;
	FDefensivePowerHandler OnDefensivePowerChanged;
	FHealthPointHandler OnHealthPointChanged;
	FManaPointHandler OnManaPointChanged;
	FManaRegenerationHandler OnManaRegenerationChanged;
	FSpeedHandler OnSpeedChanged;
	FLifeStealHandler OnLifeStealChanged;
	FHealthPointPercentageHandler OnHealthPointPercentageChanged;
	FManaPointPercentageHandler OnManaPointPercentageChanged;
	
	FCurrentHealthPointHandler OnCurrentHealthPointChanged;
	FCurrentManaPointHandler OnCurrentManaPointChanged;
	
	FOnDeathDelegate OnDeath;

	void ApplyDamage(int32 InDamage);
	void RecoverHealth(int32 Amount);
	void ConsumeMana(int32 Amount);
	void RecoverMana(int32 Amount);
	
	void ApplyEquipmentStats(class UVMEquipment* Equipment);
	void RemoveEquipmentStats(class UVMEquipment* Equipment);

	void ApplyAdditiveModifier(FHeroStat InStat);
	void ApplyMultiplicativeModifier(FHeroStat InStat);

	void InitBaseStats(FHeroStat InStats);

	FORCEINLINE FHeroStat GetFinalStat() { return FinalStats; }
	FORCEINLINE FHeroStat GetCurStat() { return CurStats; }

	FORCEINLINE FHeroStat GetStat() { return CurStats; } // Legacy
	FORCEINLINE FHeroStat GetDefaultStat() { return FinalStats; } // Legacy

protected:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void CalcFinalStats();

protected:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat BaseStats;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat EquipmentStats;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat AdditiveModifier;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat MultiplicativeModifier;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat FinalStats;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat CurStats;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	float TimeProgress;
};
