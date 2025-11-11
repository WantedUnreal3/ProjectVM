// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "Interface/EnemyHealInterface.h"

#include "VMEnemySpawnBase.generated.h"

UCLASS()
class PROJECTVM_API AVMEnemySpawnBase : public ACharacter
	, public IEnemyHealInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVMEnemySpawnBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual void OnHealHp(float HealGauge) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float MaxHp = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHp = 1;

private:
	void InitDefaultAssetSetting();
};
