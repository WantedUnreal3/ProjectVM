// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/VMInteractableInterface.h"
#include "VMItemCube.generated.h"

UCLASS()
class PROJECTVM_API AVMItemCube : public AActor, public IVMInteractableInterface
{
	GENERATED_BODY()

public:
	AVMItemCube();
	virtual void Interact() override;

	void Setup();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void CreateEffectPath();
	void VerticalMove();
	void Rotate();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemCube")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemCube")
	TObjectPtr<class UStaticMeshComponent> ItemCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UInteractComponent> InteractComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TObjectPtr<class UVMEquipment> Equipment; 

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemCube")
	TObjectPtr<class UMaterialInstanceDynamic> ItemMaterialInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemCube")
	float TimeProgress;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USplineComponent> SplinePath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Projectile, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UNiagaraComponent> Effect;
};
