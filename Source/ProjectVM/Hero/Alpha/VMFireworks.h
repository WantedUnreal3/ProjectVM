// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/SkillBase.h"
#include "VMFireworks.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMFireworks : public USkillBase
{
	GENERATED_BODY()

public:
	UVMFireworks(const FObjectInitializer& ObjectInitializer);

	virtual void ActivateSkill(class AVMCharacterHeroBase* InOwner, class UVMHeroStatComponent* StatComp) override;

protected:
	void StartWideshot();
	void SpawnEffect();
	void AreaAttack();
	void EndWideshot();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AVMCharacterHeroBase> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AVMFireworksEffect> Effect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	int32 Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	FTimerHandle AttackHandle;
};
