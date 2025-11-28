// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMAOEHeal.generated.h"

UCLASS()
class PROJECTVM_API AVMAOEHeal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMAOEHeal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Collision")
	TObjectPtr<class USphereComponent> SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Collision")
	TArray<TObjectPtr<AActor>> InnerActorArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UParticleSystemComponent> AOEEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Member | Effect", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USoundBase> AOESound;

	void ProcessHeal();

	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	float DeltaTick = 1.0f;
	float MaxTime = 10.0f;
	float TotalTime = 0.0f;

	float HpGauge = 1.0f;
};
