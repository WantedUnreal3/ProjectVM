// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VMStatChangeable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVMStatChangeable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVM_API IVMStatChangeable
{
	GENERATED_BODY()

public:
	virtual void HealthPointChange(float Amount, AActor* Causer) = 0;
};
