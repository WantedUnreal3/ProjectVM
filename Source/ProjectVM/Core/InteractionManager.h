// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InteractionManager.generated.h"

UCLASS()
class PROJECTVM_API UInteractionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	//상호작용 가능한 액터 등록 
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void RegisterInteractable(AActor* InteractableActor);

	//상호작용 가능한 액터 제거
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void UnregisterInteractable(AActor* InteractableActor);

	void Interact();
private:

	//상호작용 가능한 액터 배열
	UPROPERTY()
	TArray<TObjectPtr<AActor>> InteractableActors;
};

