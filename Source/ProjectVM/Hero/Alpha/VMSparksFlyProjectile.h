// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMSparksFlyProjectile.generated.h"

UCLASS()
class PROJECTVM_API AVMSparksFlyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AVMSparksFlyProjectile();
	
	void InitProjectile(AActor* InOwner, AActor* InTarget, int32 InDamage);
	UFUNCTION()
	void HitTarget(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
	void CreateProjectilePath(AActor* InOwner, AActor* InTarget);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> SplinePath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> SparksFlyEffect;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AActor> Target;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	float Progress;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
	int32 Damage;
};
