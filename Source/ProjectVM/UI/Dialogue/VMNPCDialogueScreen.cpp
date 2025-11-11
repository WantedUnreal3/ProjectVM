// Fill out your copyright notice in the Description page of Project Settings.


#include "VMNPCDialogueScreen.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ListView.h"

void UVMNPCDialogueScreen::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (DialogueOptionList)
	{
		DialogueOptionList->SetVisibility(ESlateVisibility::Collapsed);
	}
}
