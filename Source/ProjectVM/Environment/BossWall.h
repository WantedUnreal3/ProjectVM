// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossWall.generated.h"

UCLASS()
class PROJECTVM_API ABossWall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossWall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "BossWall")
	void StartLoweringWall();

	UFUNCTION(BlueprintImplementableEvent, Category = "BossWallUp")
	void StartUpperingWall();

	UFUNCTION(BlueprintImplementableEvent, Category = "BossWall")
	void StartFireGimmick();

	UFUNCTION(BlueprintImplementableEvent, Category = "BossWall")
	void EndFireGimmick();
};
