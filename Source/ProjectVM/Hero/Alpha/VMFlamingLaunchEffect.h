// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMFlamingLaunchEffect.generated.h"

UCLASS()
class PROJECTVM_API AVMFlamingLaunchEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	AVMFlamingLaunchEffect();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> Effect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	float TimeProgress;
};
