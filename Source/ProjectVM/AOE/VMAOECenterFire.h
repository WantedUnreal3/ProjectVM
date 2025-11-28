// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMAOECenterFire.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAOEBigSphereOverlapActorDelegate, AActor*, Target);

UCLASS()
class PROJECTVM_API AVMAOECenterFire : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMAOECenterFire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void TriggerSpawnAOESphere();

	void SpawnAOESphere();

	void DestroyWrapper();

	FTimerHandle TimeHandle;
	FTimerHandle FinishTimeHandle;

public:
	void SetOverlapTick(float InOverlapTick) { OverlapTick = InOverlapTick; }
	void SetMaxDurationTime(float InMaxDurationTime) { MaxDurationTime = InMaxDurationTime; }

public:
	UFUNCTION()
	void BroadCastFireOverlapActor(AActor* Actor);

public:
	FOnAOEBigSphereOverlapActorDelegate OnAOESphereOverlapActor;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Mesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> SceneComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Member, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystem> AOEEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Member, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> AOESound;

	float OverlapTick = 1;
	float MaxDurationTime = 10;
	float Radius = 3000.0;

	bool bDrawDebugSphere = true;

};
