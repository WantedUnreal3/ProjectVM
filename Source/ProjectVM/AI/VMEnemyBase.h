// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/VMAIEnemyBaseInterface.h"

#include "VMEnemyBase.generated.h"

//#define PAWNSENSING 1



UCLASS()
class PROJECTVM_API AVMEnemyBase : public ACharacter,
	public IVMAIEnemyBaseInterface
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

#pragma region IVMAIEnemyBaseInterface
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;
#pragma endregion

	//// Pawn Sensing 붙이기
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	//TObjectPtr<class UPawnSensingComponent> PawnSensing;
	//
	//UFUNCTION()
	//void OnSeePawn(APawn* Pawn);

	//UFUNCTION()
	//void OnHearPawn(APawn* InstigatorPawn, const FVector& Location, float Volume);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> NormalAttackMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> SkillAttack1Montage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> SkillAttack2Montage;
	//TObjectPtr<class UAnimSequence> NormalAttackAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Anim)
	TObjectPtr<class UAnimMontage> DeadMontage;


	// Stat
	FORCEINLINE float	GetCurrentHp()					{ return CurrentHp; }
	FORCEINLINE void	SetCurrentHp(float InCurrentHp) { CurrentHp = InCurrentHp; }

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float CurrentHp;
};
