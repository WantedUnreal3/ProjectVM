// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMAOEFire.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAOESphereOverlapActorDelegate, AActor*, Target);

UCLASS()
class PROJECTVM_API AVMAOEFire : public AActor
{
	GENERATED_BODY()
	
public:	
	AVMAOEFire();

public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

public:
	void TriggerSpawnAOESphere();

	void SpawnAOESphere();

	void DestroyWrapper();

	FTimerHandle TimeHandle;
	FTimerHandle FinishTimeHandle;

public:
	void SetOverlapTick(float InOverlapTick) { OverlapTick = InOverlapTick;}
	void SetMaxDurationTime(float InMaxDurationTime) { MaxDurationTime = InMaxDurationTime;}

public:
	UFUNCTION()
	void BroadCastFireOverlapActor(AActor* Actor);

public:
	FOnAOESphereOverlapActorDelegate OnAOESphereOverlapActor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Member, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> AOEEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Member, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> AOESound;

	float OverlapTick = 1;
	float MaxDurationTime = 3.5;
	float Radius = 300.0;

	bool bDrawDebugSphere = true;
};
