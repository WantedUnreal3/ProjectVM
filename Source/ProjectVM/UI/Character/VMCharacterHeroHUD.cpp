// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Character/VMCharacterHeroHUD.h"
#include "UI/Inventory/VMInteractionWidget.h"
#include "UI/Inventory/VMMainMenu.h"

AVMCharacterHeroHUD::AVMCharacterHeroHUD()
{

}

void AVMCharacterHeroHUD::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("QWER MainMenuClass 확인 전"));
	if (MainMenuClass)
	{
		UE_LOG(LogTemp, Log, TEXT("QWER MainMenuClass HIHI"));
		MainMenuWidget = CreateWidget<UVMMainMenu>(GetWorld(), MainMenuClass);
		MainMenuWidget->AddToViewport(5);
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (InteractionWidgetClass)
	{
		InteractionWidget = CreateWidget<UVMInteractionWidget>(GetWorld(), InteractionWidgetClass);
		InteractionWidget->AddToViewport(-1);
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}

}


void AVMCharacterHeroHUD::DisplayMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = true;
		MainMenuWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[DisplayMenu] MainMenuWidget is nullptr"));
	}

}

void AVMCharacterHeroHUD::HideMenu()
{
	if (MainMenuWidget)
	{
		bIsMenuVisible = false;
		MainMenuWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[HideMenu] MainMenuWidget is nullptr"));
	}
}


void AVMCharacterHeroHUD::ToggleMenu()
{
	if (bIsMenuVisible)
	{
		UE_LOG(LogTemp, Display, TEXT("[ToggleMenu] is true"));
		HideMenu();

		const FInputModeGameOnly InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(false);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("[ToggleMenu] is false"));
		DisplayMenu();
		const FInputModeGameAndUI InputMode;
		GetOwningPlayerController()->SetInputMode(InputMode);
		GetOwningPlayerController()->SetShowMouseCursor(true);
	}
}

void AVMCharacterHeroHUD::ShowInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AVMCharacterHeroHUD::HideInteractionWidget()
{
	if (InteractionWidget)
	{
		InteractionWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void AVMCharacterHeroHUD::UpdateInteractionWidget(const FInteractableData* InteractableData) const
{
	if (InteractionWidget)
	{
		if (InteractionWidget->GetVisibility() == ESlateVisibility::Collapsed)
		{
			InteractionWidget->SetVisibility(ESlateVisibility::Visible);
		}

		InteractionWidget->UpdateWidget(InteractableData);
	}
}