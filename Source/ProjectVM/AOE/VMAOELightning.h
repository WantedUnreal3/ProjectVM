// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VMAOELightning.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAOEThunderOverlapActorDelegate, AActor*, Target, float, Damage);

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMAOELightning : public AActor
{
	GENERATED_BODY()
	
public:
	AVMAOELightning();

public:
	virtual void BeginPlay() override;

	
public:
	void CreateLogic();

	FORCEINLINE void SetDelay(float InDelayTime) { DelayTime = InDelayTime;}

	FORCEINLINE void SetDecal(FVector InDecalSize)
	{
		DecalSize = InDecalSize;
	}

	void InitAOEPosition();
	void SpawnAOE();

public:
	FOnAOEThunderOverlapActorDelegate OnAOEThunderOverlapActor;

	UFUNCTION()
	void BroadcastOverlapActor(AActor* Actor, float InDamage);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Member | Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDecalComponent> DecalComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> ParticleSystemComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USphereComponent> SphereComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> SoundBase;

	UPROPERTY()
	float DelayTime;

	UPROPERTY()
	FTimerHandle DecalTimeHandle;

	UPROPERTY()
	FVector DecalSize;

	UPROPERTY()
	FVector DecalLocation;

	UPROPERTY()
	FVector Location;

	UPROPERTY()
	FRotator DecalRotator;

	
};
