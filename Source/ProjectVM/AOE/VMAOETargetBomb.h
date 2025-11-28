// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMAOETargetBomb.generated.h"

UCLASS()
class PROJECTVM_API AVMAOETargetBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMAOETargetBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// === Components ===
	UPROPERTY(EditAnywhere, Category = "Bomb", Meta=(AllowPrivateAccess = True))
	TObjectPtr<class UCapsuleComponent> CylinderCollision;

	UPROPERTY(EditAnywhere, Category = "Bomb", Meta = (AllowPrivateAccess = True))
	TObjectPtr<class UStaticMeshComponent> Mesh;

	// Logic
	UPROPERTY(EditAnywhere, Category = "Bomb", Meta = (AllowPrivateAccess = True))
	float ExplosionDelay = 3.0f; // 밟고 3초 후 폭발

	UPROPERTY()
	TObjectPtr<class AVMCharacterHeroBase> TriggeringCharacter;

	FTimerHandle ExplosionTimerHandle;

	// Callbacks
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void Detonate();

	float Radius = 300.0f;
};
