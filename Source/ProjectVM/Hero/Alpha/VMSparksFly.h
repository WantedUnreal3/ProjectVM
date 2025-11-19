// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/SkillBase.h"
#include "VMSparksFly.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMSparksFly : public USkillBase
{
	GENERATED_BODY()

public:
	UVMSparksFly(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp) override;

private:
	void StartSpawnProjectile();
	void SpawnProjectile();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AActor> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> Targets;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	FTimerHandle ProjectileTimerHandle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	int32 ProjectileCountToSpawn;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	int32 ProjectileCount;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	int32 ProjectileDamage;
};
