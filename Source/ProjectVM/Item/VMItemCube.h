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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemCube")
	TObjectPtr<class UBoxComponent> Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemCube")
	TObjectPtr<class UStaticMeshComponent> ItemCube;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
	TObjectPtr<class UInteractComponent> InteractComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TObjectPtr<class UVMEquipment> Equipment; 

	TObjectPtr<class UMaterialInstanceDynamic> ItemMaterialInstance;
};
