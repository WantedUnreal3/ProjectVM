// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VMPlayer.generated.h"

UCLASS()
class PROJECTVM_API AVMPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AVMPlayer();

#pragma region 엔진_제공_함수
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
#pragma endregion 

#pragma region Attack애니메이션함수
	void PlayAttackAnim();

	void PlayLaserAttackAnim();

public:
	void LaserAttackHitCheck();
#pragma endregion 

#pragma region Input_함수
protected:
	void Move(const struct FInputActionValue& Value);

	void Look(const struct FInputActionValue& Value);

	virtual void Jump() override;

	UFUNCTION()
	void LeftAttack();

	UFUNCTION()
	void MiddleAttack();

	UFUNCTION()
	void RightAttack();
#pragma endregion 


	// 가장 가까운 적 찾는 함수.
	AActor* FindClosestEnemy();

#pragma region Input_변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	/** LeftMouseButton Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftMouseAction;

	/** MiddleMouseButton Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MiddleMouseAction;

	/** RightMouseButton Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightMouseAction;
#pragma endregion 

#pragma region PawnSensing_변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Noise, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPawnNoiseEmitterComponent> PawnNoiseEmitter;
#pragma endregion


#pragma region Attack_변수
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Noise, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> AttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Noise, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> LaserAttackMontage;
#pragma endregion

#pragma region Ally_Section
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SpawnAllyAction;

	void SpawnAllyActor();
#pragma endregion

public:
	UFUNCTION()
	FORCEINLINE void SetCurrentHP(float InCurrentHp) { 
		CurrentHp = FMath::Clamp(InCurrentHp, 0, MaxHp); }


	FORCEINLINE void HealHp(float InHealHp) {
		CurrentHp = FMath::Clamp(CurrentHp + InHealHp, 0, MaxHp);
	}

	FORCEINLINE void HealMp(float InHealMp) {
		CurrentMp = FMath::Clamp(CurrentMp + InHealMp, 0, MaxMp);
	}

	FORCEINLINE float GetCurrentHp() const { return CurrentHp; }
	FORCEINLINE float GetMaxHp() const { return MaxMp; }

	FORCEINLINE float GetCurrentMp() const { return CurrentMp; }
	FORCEINLINE float GetMaxMp() const { return MaxMp; }


	FORCEINLINE float GetMoveSpeed() const { return MoveSpeed; }
	FORCEINLINE float GetAttackDamage() const { return AttackDamageAmount; }
	FORCEINLINE float GetAttackSpeed() const { return AttackSpeed; }

	FORCEINLINE void AddMoveSpeed(float InMoveSpeed)  {  MoveSpeed += InMoveSpeed; }
	FORCEINLINE void AddAttackDamage(float InAttackDamage)  { AttackDamageAmount += InAttackDamage; }
	FORCEINLINE void AddAttackSpeed(float InAttackSpeed)  { AttackSpeed += InAttackSpeed; }


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float CurrentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float CurrentMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float MaxMp;


	// MoveSpeed, AttackDamage, 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float AttackDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;
};
