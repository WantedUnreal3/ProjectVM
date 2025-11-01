// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VMAIEnemyBaseInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVMAIEnemyBaseInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVM_API IVMAIEnemyBaseInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

#pragma region 인터페이스_함수
public:
	// Enemy가 필수적으로 구현해야할 함수를 선언.
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;
#pragma endregion
};
