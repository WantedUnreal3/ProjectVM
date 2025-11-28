// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingLaserPillar.generated.h"

UCLASS()
class PROJECTVM_API ARotatingLaserPillar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARotatingLaserPillar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    /** 세로 기둥 메쉬 */
    /** 회전 기준 */
    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UStaticMeshComponent> PillarMesh;


    /** 레이저 메쉬 */
    /*UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UStaticMeshComponent> LaserMesh;*/

    UPROPERTY(VisibleAnywhere)
    TObjectPtr<class UParticleSystemComponent> LaserParticle;

    /** 데미지 충돌체 */
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* DamageCollider;

    /** 회전 속도 (도/초) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
    float RotationSpeed;

    /** 데미지량 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
    float DamageAmount;

    /** 지속 데미지 간격 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
    float DamageInterval;

private:
    /** 현재 레이저에 겹쳐 있는 액터 */
    TSet<AActor*> OverlappingActors;

    /** 데미지 타이머 */
    FTimerHandle DamageTimerHandle;

    /** 데미지 적용 함수 */
    void DealDamage();

    /** Overlap 이벤트 */
    UFUNCTION()
    void OnDamageOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnDamageEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
