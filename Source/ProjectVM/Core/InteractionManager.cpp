// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/InteractionManager.h"
#include "Core/VMInteractableInterface.h"
#include "Game/VMRPGPlayerController.h"
#include "Kismet/GameplayStatics.h"
void UInteractionManager::RegisterInteractable(AActor* InteractableActor)
{
	if (!InteractableActor)
		return;

	// 중복 등록 방지
	if (!InteractableActors.Contains(InteractableActor))
	{
		InteractableActors.Add(InteractableActor);
		UE_LOG(LogTemp, Log, TEXT("InteractManager: Register Actor"));

		//처음으로 상호작용할 액터가 생기면 상호작용키 UI에 표시
		if (InteractableActors.Num() == 1)
		{
			AVMRPGPlayerController* PC = Cast< AVMRPGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
			if (PC == nullptr)
			{
				return;
			}
			PC->ToggleInteractKey(true);
		}
	}
}

void UInteractionManager::UnregisterInteractable(AActor* InteractableActor)
{
	if (!InteractableActor)
		return;

	InteractableActors.Remove(InteractableActor);
	UE_LOG(LogTemp, Log, TEXT("InteractManager: Unregister Actor"));

	//상호작용할 액터가 없으면 UI 삭제
	if (InteractableActors.Num() == 0)
	{

		AVMRPGPlayerController* PC = Cast< AVMRPGPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		if (PC == nullptr)
		{
			return;
		}

		PC->ToggleInteractKey(false);
	}
}

void UInteractionManager::Interact()
{
	if (InteractableActors.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("InteractManager: Actors is empty"));
		return;
	}
	UE_LOG(LogTemp, Log, TEXT("InteractManager: Interact"));

	TObjectPtr<AActor> ActorPtr = InteractableActors[0];

	if (ActorPtr->GetClass()->ImplementsInterface(UVMInteractableInterface::StaticClass()))
	{
		IVMInteractableInterface* Interactable = Cast<IVMInteractableInterface>(ActorPtr);
		if (Interactable)
		{
			Interactable->Interact();
			UE_LOG(LogTemp, Log, TEXT("Interact called on %s"), *ActorPtr->GetName());
		}
	}
}