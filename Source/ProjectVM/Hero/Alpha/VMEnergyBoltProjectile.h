// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMEnergyBoltProjectile.generated.h"

UCLASS()
class PROJECTVM_API AVMEnergyBoltProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AVMEnergyBoltProjectile();

public:
	void BindTarget(AActor* InTarget);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void Homing();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> SplinePath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> SphereMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AActor> Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float Progress;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	//ObjectPtr<class UNiagaraComponent> EnergyBoltEffect;
};
