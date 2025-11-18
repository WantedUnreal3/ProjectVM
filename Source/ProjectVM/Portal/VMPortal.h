// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/VMInteractableInterface.h"
#include "VMPortal.generated.h"

UCLASS()
class PROJECTVM_API AVMPortal : public AActor, public IVMInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMPortal();
	virtual void Interact() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//맵 이동
	void TeleportPlayerToMap();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UNiagaraComponent> PortalNiagaraSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UNiagaraComponent> EffectLineNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UInteractComponent> InteractComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetMap")
	FString TargetLevelPath = "/Game/Project/Map/TestBossMap.TestBossMap";

	TObjectPtr < class ULevelStreamingDynamic> LoadedLevel = nullptr;

	//이펙트 타이머
	FTimerHandle EffectTimerHandle;
}; 