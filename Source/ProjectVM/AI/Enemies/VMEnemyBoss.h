// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/VMSummonInterface.h"
#include "Interface/EnemyHealInterface.h"
#include "Interface/VMStatChangeable.h"

#include "VMEnemyBoss.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FHealthPointPercentageHandler, float /* HPPercentage */);

UCLASS()
class PROJECTVM_API AVMEnemyBoss : public ACharacter
	//, public IVMSummonInterface
	, public IEnemyHealInterface
	, public IVMStatChangeable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVMEnemyBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void InitDefaultSetting();
	void LoadAsset();


	// Skill Summon
public:
	void ActivateSummonMontage();
	void SummonMinion(FVector Pos);

	void DeactivateSummonMontage();

//public:
//	virtual void SummonMinions() override;
//
//	virtual float GetSummonRange() const override;
//	virtual int32 GetNumOfSummon() const override;

public:
	virtual void OnHealHp(float HealGauge) override;

public:
	void ActivateSummonMinion();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHp = 1500;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float PhaseMaxHp = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float PhaseMinHp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHp = 1500;


	virtual void HealthPointChange(float Amount, AActor* Causer) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> SummonMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> MoveMontage;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class AVMEnemySpawnBase>> EnemySpawnArray;

	 
public:
	FHealthPointPercentageHandler OnHealthPointPercentageChanged;

	void SaveAllSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, meta = (AllowPrivateAccess = true))
	TArray<class AActor*> Spawners;

	TArray<float> HPPhase;
	int32 PhaseIndex;

	void UpdatePhase();

public:
	void ClearDelegate();

	// Phase 2 돌입 시 사용할 변수
	UPROPERTY()
	TObjectPtr<class ABossWall> BossWall;

	UPROPERTY()
	TObjectPtr<class ABossWater> BossWater;
};
