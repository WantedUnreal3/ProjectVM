// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/VMAIEnemyBaseInterface.h"
#include "Interface/VMStatChangeable.h"
#include "Interface/VMLaserAttackInterface.h"

#include "VMEnemyBase.generated.h"

UCLASS()
class PROJECTVM_API AVMEnemyBase : public ACharacter
	, public IVMStatChangeable
	, public IVMLaserAttackInterface
	, public IVMAIEnemyBaseInterface
{
	GENERATED_BODY()

#pragma region 특수_맴버_함수
public:
	AVMEnemyBase();
#pragma endregion

#pragma region 엔진_제공_함수
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion

	float DeltaTimer = 2.0f;

#pragma region IVMAIEnemyBaseInterface
	virtual float GetAIMoveSpeed() const override;
	virtual float GetAIAttackRange() const override;
	virtual float GetAITurnSpeed() const override;
	virtual float GetAIAttackRadius() const override;
	virtual float GetAINormalAttackDamage() const override;
	virtual float GetAIAttackSpeed() const override;

	virtual void NormalAttack() override;
	virtual void NormalAttackCheck() override;
#pragma endregion

	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> NormalAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> SkillAttack1Montage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> SkillAttack2Montage;
	//TObjectPtr<class UAnimSequence> NormalAttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> DeadMontage;



#pragma region 임시 공격 테스트용
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Noise, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> LaserAttackMontage;
#pragma endregion

#pragma region EnemyStatComponent 임시용
public: // Stat	
	FORCEINLINE float	GetCurrentHp()					{ return CurrentHp; }
	FORCEINLINE void	SetCurrentHp(float InCurrentHp) { CurrentHp = InCurrentHp; }
	FORCEINLINE float	GetMaxHp()						{ return MaxHp; }
	FORCEINLINE void	SetMaxHp(float InMaxHp)			{ MaxHp = InMaxHp; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Member")
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stat | Member")
	float MaxHp;
#pragma endregion

#pragma region IVMStatChangeable 인터페이스 필수 구현 함수
public:
	virtual void HealthPointChange(float Amount, AActor* Causer) override;
#pragma endregion

#pragma region IVMLaserAttackInterface 필수 구현 함수
public:
	virtual void LaserAttackHitCheck() override;
#pragma endregion

#pragma region PawnSensing
	// Pawn Sensing 붙이기
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Member | AI", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnSensingComponent> PawnSensing;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Member | Target", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class APawn> EnemyTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Member | Target", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnNoiseEmitterComponent> EmitterComp;
	
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHearPawn(APawn* InstigatorPawn, const FVector& Location, float Volume);

	void TryMakeNoise();
	void ResetSoundCooldown();

	bool bCanMakeSound = true;
	FTimerHandle MemoryTimers;
	FTimerHandle SoundTimers;

public:
	FString SectionNamePrefix = TEXT("Attack");
	int32 CurrentSectionIndex = 1;


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	//float NoiseInterval;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	//float NoiseLoudness;

	//// Perception
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	//TObjectPtr<class UAIPerceptionComponent> PerceptionComp;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	//TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	//TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;

	//// AI Perception 감지 이벤트
	//UFUNCTION()
	//void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);


	//// Noise 발생 함수
	//void MakeNoiseForNPC(float Loudness = 2000.f);
#pragma endregion
};
