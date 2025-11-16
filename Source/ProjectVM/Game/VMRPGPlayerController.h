// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/GameEnums.h"
#include "VMRPGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API AVMRPGPlayerController : public APlayerController
{
	GENERATED_BODY()

	AVMRPGPlayerController();

public:
	UFUNCTION()
	void ShowScreen(EScreenUIType ScreenType);

	UFUNCTION()
	void HideAllScreen();

	UFUNCTION()
	UUserWidget* GetScreen(EScreenUIType ScreenType);

protected:
	virtual void BeginPlay() override;

protected:
	//메인 게임 스크린
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UVMGameScreen> VMGameScreenClass;
	UPROPERTY()
	class UVMGameScreen* VMGameScreen;

	//NPC 대화 스크린
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UVMNPCDialogueScreen> VMNPCDialogueScreenClass;
	UPROPERTY()
	class UVMNPCDialogueScreen* VMNPCDialogueScreen;

	//상점 스크린
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UVMShopScreen> VMShopScreenClass;
	UPROPERTY()
	class UVMShopScreen* VMShopScreen;

	UPROPERTY()
	TMap<EScreenUIType, UUserWidget*> ScreenUIMap;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UVMHeroStatusWidget> VMHeroStatusWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<class UVMHeroStatusWidget> VMHeroStatusWidget;
};