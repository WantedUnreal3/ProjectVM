// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMFireworksEffect.generated.h"

UCLASS()
class PROJECTVM_API AVMFireworksEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	AVMFireworksEffect();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> Effect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	float TimeProgress;
};
