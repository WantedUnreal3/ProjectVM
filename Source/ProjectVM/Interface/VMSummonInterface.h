// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VMSummonInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVMSummonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECTVM_API IVMSummonInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SummonMinions() = 0;

	virtual float GetSummonRange() const = 0;
	virtual int32 GetNumOfSummon() const = 0;
};
