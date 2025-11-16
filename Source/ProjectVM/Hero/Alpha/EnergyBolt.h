// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/SkillBase.h"
#include "EnergyBolt.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UEnergyBolt : public USkillBase
{
	GENERATED_BODY()

public:
	UEnergyBolt(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp) override;

private:
	void StartSpawnProjectile();
	void SpawnProjectile();

private:
	TObjectPtr<class AActor> Owner;
	TArray<AActor*> Targets;
	
	FTimerHandle ProjectileTimerHandle;
	int32 ProjectileCountToSpawn;
	int32 ProjectileCount;
	int32 ProjectileDamage;
};