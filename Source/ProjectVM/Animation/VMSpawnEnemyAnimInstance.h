// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "VMSpawnEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMSpawnEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UVMSpawnEnemyAnimInstance();


	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helper)
	TObjectPtr<class ACharacter> OwningActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Helper)
	TObjectPtr<class UCharacterMovementComponent> CharacterMovementComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
	float MoveDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
	float MoveSpeed;
};
