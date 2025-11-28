// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VMAOEBomb.generated.h"

UCLASS()
class PROJECTVM_API AVMAOEBomb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMAOEBomb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UStaticMeshComponent> MeshComp;

    UPROPERTY(EditDefaultsOnly, Category = "Effect")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Effect")
    USoundBase* ExplosionSound;

    FTimerHandle ExplosionTimerHandle;

    /** Called when bomb hits something */
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    /** Explode bomb */
    void Explode();

    bool flag = false;

    float ExplosionTime;
};
