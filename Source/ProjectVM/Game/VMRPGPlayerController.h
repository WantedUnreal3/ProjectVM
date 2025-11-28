// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Core/GameEnums.h"

#include "UI/Inventory/VMInventoryPanel.h"
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

	void ToggleInteractKey(bool bIsVisible);

	// 인벤토리 열기/닫기 함수
	UFUNCTION(BlueprintCallable)
	void OpenInventory();

	UFUNCTION(BlueprintCallable)
	void CloseInventory();

	// 보스
	UFUNCTION(BlueprintCallable)
	void ShowBossStatusWidget();

	UFUNCTION(BlueprintCallable)
	void HideBossStatusWidget();
	
	void ShowGameOverUI();
	void HideGameOverUI();

	void ShowQuestClearUI();
	void HideQuestClearUI();
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

	// 인벤토리 패널 UMG 클래스
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UVMInventoryPanel> InventoryPanelClass;

	// 생성된 패널 인스턴스
	UPROPERTY()
	UVMInventoryPanel* InventoryPanel;


	// 보스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UVMHeroStatusWidget> VMBossStatusWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<class UVMHeroStatusWidget> VMBossStatusWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> GameOverWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> GameOverWidget;

	//퀘스트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> QuestClearOverlayClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	TObjectPtr<class UUserWidget> QuestClearOverlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<class UVMSkillsCooldownWidget> SkillsCooldownWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<class UVMSkillsCooldownWidget> SkillsCooldownWidget;
};