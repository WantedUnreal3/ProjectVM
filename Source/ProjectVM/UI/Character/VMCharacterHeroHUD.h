// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VMCharacterHeroHUD.generated.h"

struct FInteractableData;
class UVMMainMenu;
class UVMInteractionWidget;
class UVMInventoryPanel;

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMCharacterHeroHUD : public AHUD
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> MainMenuClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UVMInteractionWidget> InteractionWidgetClass;

	bool bIsMenuVisible;

	UFUNCTION(BlueprintCallable)
	UVMInventoryPanel* GetInventoryPanel() const { return InventoryPanel; }

public:
	AVMCharacterHeroHUD();

	void DisplayMenu();
	void HideMenu();
	void ToggleMenu();

	void ShowInteractionWidget();
	void HideInteractionWidget();
	void UpdateInteractionWidget(const FInteractableData* InteractableData) const;

protected:
	UPROPERTY()
	UVMMainMenu* MainMenuWidget;

	UPROPERTY()
	UVMInteractionWidget* InteractionWidget;

	// UMG 클래스 지정용
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UVMInventoryPanel> InventoryPanelClass;

	// 실제 인스턴스
	UPROPERTY()
	UVMInventoryPanel* InventoryPanel;

protected:
	virtual void BeginPlay() override;
	

};
