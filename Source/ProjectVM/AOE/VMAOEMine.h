// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMAOEMine.generated.h"

UCLASS()
class PROJECTVM_API AVMAOEMine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMAOEMine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Mine")
	TObjectPtr<class UCapsuleComponent> Collision;

	UPROPERTY(VisibleAnywhere, Category = "Mine")
	TObjectPtr<class UStaticMeshComponent> MineMesh;

	// === Logic ===

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult );

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 폭발
	void Detonate();

	// 폭탄 해제.
	void Disarm();

	UPROPERTY()
	TObjectPtr<class AVMCharacterHeroBase> OverlappingCharacter;

	FTimerHandle DisarmTimerHandle;

	bool bIsDisarmed;

	float Radius = 300;
};
