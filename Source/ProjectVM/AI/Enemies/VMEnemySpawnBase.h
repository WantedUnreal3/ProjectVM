// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/EnemyHealInterface.h"
#include "Interface/VMStatChangeable.h"
#include "Interface/VMAIEnemyBaseInterface.h"

#include "Quest/VMQuestManager.h"

#include "Core/VMMonsterEnums.h"

#include "VMEnemySpawnBase.generated.h"

UCLASS()
class PROJECTVM_API AVMEnemySpawnBase : public ACharacter
	, public IEnemyHealInterface
	, public IVMStatChangeable
	, public IVMAIEnemyBaseInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVMEnemySpawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//virtual void HealthPointChange(float Amount, AActor* Causer) override;


#pragma region EnemyStatComponent 임시용
public: // Stat	
	FORCEINLINE float	GetCurrentHp() { return CurrentHp; }
	FORCEINLINE void	SetCurrentHp(float InCurrentHp) { CurrentHp = InCurrentHp; }
	FORCEINLINE float	GetMaxHp() { return MaxHp; }
	FORCEINLINE void	SetMaxHp(float InMaxHp) { MaxHp = InMaxHp; }
#pragma endregion

#pragma region IVMStatChangeable
public:
	virtual void HealthPointChange(float Amount, AActor* Causer) override;
#pragma endregion 

public:
	virtual void OnHealHp(float HealGauge) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHp = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHp = 1;

private:
	void InitDefaultAssetSetting();

public:
	FORCEINLINE EMonsterName GetMonsterType() { return MonsterType; }
	FORCEINLINE void SetMonsterType(EMonsterName InMonsterType) { MonsterType = InMonsterType; }


	virtual float GetAIMoveSpeed() const { return MoveSpeed; }
	virtual float GetAIAttackRadius() const { return AttackRadius; }
	virtual float GetAIAttackSpeed() const { return AttackSpeed; }
	virtual float GetAIAttackRange() const { return AttackRange; }
	virtual float GetAITurnSpeed() const { return TurnSpeed; }
	virtual float GetAINormalAttackDamage() const { return AttackDamage; }

	virtual void NormalAttack() override;
	virtual void NormalAttackCheck() override;
	virtual void PlayNormalAttackMontage();


	UAnimMontage* GetNormalMontage() const { return NormalAttackMontage;}

protected:
	EMonsterName MonsterType = EMonsterName::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> NormalAttackMontage;


protected:
	float MoveSpeed = 100.0f;
	float AttackRadius = 50.0f;
	float AttackSpeed = 1.0f;
	float AttackRange = 100.0f;
	float TurnSpeed = 2.0f;
	float AttackDamage = 2.0f;
};
