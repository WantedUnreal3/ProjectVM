// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/VMAIEnemyBaseInterface.h"

#include "VMAllyBase.generated.h"

UENUM()
enum class EAllyMontage
{
	Normal,
	Straight,
	Homing,
	Heal,
	Dead
};

UCLASS()
class PROJECTVM_API AVMAllyBase : public ACharacter,
	public IVMAIEnemyBaseInterface
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	AVMAllyBase();

#pragma region 엔진_제공_함수
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion 

#pragma region IVMAIEnemyBaseInterface
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
#pragma endregion

#pragma region Skill
public:
	void HealOwnerHp();
	void HealOwnerMp();

	void ActivateHpParticle();
	void DeactivateHpParticle();

	void ActivateMpParticle();
	void DeactivateMpParticle();

	// Buff
	void ApplyOwnerDamageBuff();
	void ActivateDamageBuffParticle();
	void DeactivateDamageBuffParticle();
#pragma endregion

public:
	AActor* GetOwnerTarget() const;
	AActor* GetEnemyTarget() const;
	void SetOwnerTarget(AActor* InOwnerTarget);
	void SetEnemyTarget(AActor* InEnemyTarget);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> HealMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> BuffMontage;

	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> SkillAttack2Montage;
	*/
	
	//TObjectPtr<class UAnimSequence> NormalAttackAnim;

	// Owner 타겟
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Target)
	TObjectPtr<AActor> OwnerTarget;

	// EnemyTarget
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Target)
	TObjectPtr<AActor> EnemyTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UNiagaraComponent> HealHpNiagaraSystemEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UNiagaraComponent> HealMpNiagaraSystemEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UNiagaraComponent> DamageNiagaraSystemEffect;

	// 임시 Stat 저장. TODO: 컴포넌트로 빼야함.
	float CurrentLivedTime = 120.0f;
	float MaxLivedTime = 120.0f;
};
