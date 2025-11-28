// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMAOEFloatingBomb.generated.h"

UCLASS()
class PROJECTVM_API AVMAOEFloatingBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMAOEFloatingBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floating")
    float RotationSpeed = 10.f;

    // Explosion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosionRadius = 500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosionDamage = 50.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    TObjectPtr<class UParticleSystem> ExplosionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    TObjectPtr<class USoundBase> ExplosionSound;

    // === Components ===
    UPROPERTY(EditAnywhere, Category = "Bomb", Meta = (AllowPrivateAccess = True))
    TObjectPtr<class USphereComponent> CollisionSphere;

    UPROPERTY(EditAnywhere, Category = "Bomb", Meta = (AllowPrivateAccess = True))
    TObjectPtr<class UStaticMeshComponent> Mesh;

private:
    UPROPERTY()
    TObjectPtr<class AVMCharacterHeroBase> PlayerActor;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
    UFUNCTION(BlueprintCallable, Category = "Explosion")
    void Explode();

    float DeltaTickTime = 0.2f;
    float TotalTime = 0;
};
