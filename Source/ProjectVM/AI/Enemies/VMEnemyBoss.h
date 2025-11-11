// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/VMSummonInterface.h"
#include "Interface/EnemyHealInterface.h"

#include "VMEnemyBoss.generated.h"

UCLASS()
class PROJECTVM_API AVMEnemyBoss : public ACharacter
	//, public IVMSummonInterface
	, public IEnemyHealInterface
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
	float MaxHp = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHp = 1;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Anim, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAnimMontage> SummonMontage;
};
