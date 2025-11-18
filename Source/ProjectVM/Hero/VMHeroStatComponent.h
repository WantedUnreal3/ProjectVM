// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/HeroStat.h"
#include "Components/ActorComponent.h"
#include "VMHeroStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FStatHandler, FHeroStat /* CurrentStat */);
DECLARE_MULTICAST_DELEGATE_OneParam(FAttackPowerHandler, int32 /* CurrentAtk */);
DECLARE_MULTICAST_DELEGATE_OneParam(FDefensivePowerHandler, int32 /* CurrentDef */);
DECLARE_MULTICAST_DELEGATE_OneParam(FHealthPointHandler, int32 /* CurrentHP */);
DECLARE_MULTICAST_DELEGATE_OneParam(FManaPointHandler, int32 /* CurrentMP */);
DECLARE_MULTICAST_DELEGATE_OneParam(FManaRegenerationHandler, int32 /* CurrentMP */);
DECLARE_MULTICAST_DELEGATE_OneParam(FSpeedHandler, int32 /* CurrentSpeed */);
DECLARE_MULTICAST_DELEGATE_OneParam(FLifeStealHandler, int32 /* CurrentSpeed */);

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
	FOnDeathDelegate OnDeath;
	
	void ApplyDamage(int32 InDamage);
	void RecoverHealth(int32 Amount);
	void ConsumeMana(int32 Amount);
	void RecoverMana(int32 Amount);
	void ApplyDefaultStat(FHeroStat InStat);
	void ApplyStat(FHeroStat InStat);
	
	void ApplyEquipmentStats(class UVMEquipment* Equipment);
	void RemoveEquipmentStats(class UVMEquipment* Equipment);

	FORCEINLINE FHeroStat GetFinalStat() { return FinalStats; }
	FORCEINLINE FHeroStat GetCurStat() { return CurStats; }

	FORCEINLINE FHeroStat GetStat() { return CurStats; } // Legacy
	FORCEINLINE FHeroStat GetDefaultStat() { return FinalStats; } // Legacy


protected:
	virtual void InitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void CalcFinalStats();

protected:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat BaseStats;

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
