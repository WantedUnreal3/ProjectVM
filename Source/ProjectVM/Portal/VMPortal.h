// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/VMInteractableInterface.h"

#include "NavigationSystem.h"
#include "NavigationSystemTypes.h" 

#include "VMPortal.generated.h"

UCLASS()
class PROJECTVM_API AVMPortal : public AActor, public IVMInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AVMPortal();
	virtual void Interact() override;

	FORCEINLINE class UNiagaraComponent* GetPortalNiagaraSystem() const {
		return PortalNiagaraSystem;
	}

	//맵 이동
	UFUNCTION()
	virtual void TeleportPlayerToMap();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UCapsuleComponent> Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UNiagaraComponent> PortalNiagaraSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	TObjectPtr<class UNiagaraComponent> EffectLineNiagaraSystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UInteractComponent> InteractComponent;

	//TObjectPtr < class ULevelStreamingDynamic> LoadedLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	FVector MapTeleportPos = FVector(0.f, 0.f, 0.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	FVector PlayerTeleportPos = FVector(0.f, 0.f, 0.f);
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Position")
	//FRotator PlayerTeleportRot = FRotator(0.f, 0.f, 0.f);

	//이펙트 타이머
	FTimerHandle EffectTimerHandle;
}; 