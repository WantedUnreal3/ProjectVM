// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/VMAIEnemyBaseInterface.h"

#include "VMEnemyBase.generated.h"

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
};
