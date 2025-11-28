// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMRPGPlayerController.h"

#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"
#include "Hero/VMHeroSkillComponent.h"
#include "UI/Common/VMGameScreen.h"
#include "UI/Dialogue/VMNPCDialogueScreen.h"
#include "UI/Shop/VMShopScreen.h"
#include "UI/Stat/VMHeroStatusWidget.h"
#include "UI/Common/VMInteractKeyWidget.h"
#include "UI/Skill/VMSkillsCooldownWidget.h"

// 보스
#include "AI/Enemies/VMEnemyBoss.h"
#include "Engine/World.h"
#include "EngineUtils.h"

AVMRPGPlayerController::AVMRPGPlayerController()
{
	//게임 스크린 로드
	static ConstructorHelpers::FClassFinder<UVMGameScreen> VMGameScreenRef(TEXT("/Game/Project/UI/Common/WBP_VMGameScreen.WBP_VMGameScreen_C"));
	if (VMGameScreenRef.Succeeded())
	{
		VMGameScreenClass = VMGameScreenRef.Class;
	}

	//대화 스크린 로드
	static ConstructorHelpers::FClassFinder<UVMNPCDialogueScreen> VMNPCDialogueScreenRef(TEXT("/Game/Project/UI/Dialogue/WBP_VMNPCDialogueScreen.WBP_VMNPCDialogueScreen_C"));

	if (VMNPCDialogueScreenRef.Succeeded())
	{
		VMNPCDialogueScreenClass = VMNPCDialogueScreenRef.Class;
	}

	//상점 스크린 로드
	static ConstructorHelpers::FClassFinder<UVMShopScreen> VMShopScreenRef(TEXT("/Game/Project/UI/Shop/WBP_VMShopScreen.WBP_VMShopScreen_C"));

	if (VMShopScreenRef.Succeeded())
	{
		VMShopScreenClass = VMShopScreenRef.Class;
	}

	// 보스 스크린 로드
	static ConstructorHelpers::FClassFinder<UVMHeroStatusWidget> VMBossStatusWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project/UI/Stat/WBP_BossStatus.WBP_BossStatus_C'"));
	if (VMBossStatusWidgetRef.Succeeded())
	{
		VMBossStatusWidgetClass = VMBossStatusWidgetRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidgetRef(TEXT("/Game/Project/UI/Common/WBP_GameOverWidget.WBP_GameOverWidget_C"));
	if (GameOverWidgetRef.Succeeded())
	{
		GameOverWidgetClass = GameOverWidgetRef.Class;
	}

	//퀘스트 클리어 스크린 로드
	static ConstructorHelpers::FClassFinder<UUserWidget> QuestClearOverlayRef(TEXT("/Game/Project/UI/Quest/WBP_QuestClearOverlay.WBP_QuestClearOverlay_C"));
	if (QuestClearOverlayRef.Succeeded())
	{
		QuestClearOverlayClass = QuestClearOverlayRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<UVMSkillsCooldownWidget> SkillCooldownWidgetRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Project/UI/Skill/WBP_HeroSkillWidget.WBP_HeroSkillWidget_C'"));
	if (SkillCooldownWidgetRef.Succeeded())
	{
		SkillsCooldownWidgetClass = SkillCooldownWidgetRef.Class;
	}
}

void AVMRPGPlayerController::ShowScreen(EScreenUIType ScreenType)
{
	HideAllScreen();
	UUserWidget** FoundWidget = ScreenUIMap.Find(ScreenType);
	if (FoundWidget != nullptr)
	{
		if (*FoundWidget != nullptr)
		{
			(*FoundWidget)->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AVMRPGPlayerController::HideAllScreen()
{
	for (auto& Pair : ScreenUIMap)
	{
		if (Pair.Value)
		{
			Pair.Value->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

UUserWidget* AVMRPGPlayerController::GetScreen(EScreenUIType ScreenType)
{
	if (UUserWidget* const* Found = ScreenUIMap.Find(ScreenType))
	{
		return *Found;
	}
	return nullptr;
}

void AVMRPGPlayerController::ToggleInteractKey(bool bIsVisible)
{
	if (bIsVisible)
	{
		VMGameScreen->InteractKeyWidget->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		VMGameScreen->InteractKeyWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}


void AVMRPGPlayerController::ShowBossStatusWidget()
{
	for (TActorIterator<AVMEnemyBoss> It(GetWorld()); It; ++It)
	{
		AVMEnemyBoss* Boss = *It;
		if (Boss)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found Boss: %s"), *Boss->GetName());
			// 여기서 Boss 객체에 원하는 작업 수행
			if (VMBossStatusWidgetClass != nullptr)
			{
				VMBossStatusWidget = CreateWidget<UVMHeroStatusWidget>(this, VMBossStatusWidgetClass);
				if (VMBossStatusWidget != nullptr)
				{
					VMBossStatusWidget->AddToViewport();
					Boss->OnHealthPointPercentageChanged.AddUObject(VMBossStatusWidget, &UVMHeroStatusWidget::RefreshHealthPointBar);
					VMBossStatusWidget->RefreshHealthPointBar(1);
				}
			}
			return;
		}
	}
}

void AVMRPGPlayerController::HideBossStatusWidget()
{
	for (TActorIterator<AVMEnemyBoss> It(GetWorld()); It; ++It)
	{
		AVMEnemyBoss* Boss = *It;
		if (Boss)
		{
			if (VMBossStatusWidget != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("HideBossStatusWidget Call"));
				VMBossStatusWidget->RemoveFromParent();
				Boss->ClearDelegate();
			}
			return;
		}
	}
	
}

void AVMRPGPlayerController::ShowGameOverUI()
{
	GameOverWidget = CreateWidget<UUserWidget>(this, GameOverWidgetClass);
	if (GameOverWidget != nullptr)
	{
		GameOverWidget->AddToViewport();
	}
}

void AVMRPGPlayerController::HideGameOverUI()
{
	GameOverWidget->RemoveFromParent();
}

void AVMRPGPlayerController::ShowQuestClearUI()
{
	QuestClearOverlay = CreateWidget<UUserWidget>(this, QuestClearOverlayClass);
	if (QuestClearOverlay != nullptr)
	{
		QuestClearOverlay->AddToViewport();
	}
}

void AVMRPGPlayerController::HideQuestClearUI()
{
	QuestClearOverlay->RemoveFromParent();
}

void AVMRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Log, TEXT("PC"));
	//스크린 UI 뷰포트에 추가
	if (VMGameScreenClass != nullptr)
	{
		VMGameScreen = CreateWidget<UVMGameScreen>(this, VMGameScreenClass);
		if (VMGameScreen != nullptr)
		{
			VMGameScreen->AddToViewport();
			
			//Interact UI 숨기기
			ToggleInteractKey(true);

			ScreenUIMap.Add(EScreenUIType::GameScreen, VMGameScreen);

			VMGameScreen->InteractKeyWidget->SetVisibility(ESlateVisibility::Hidden);

			AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(GetPawn());
			Hero->GetStatComponent()->OnHealthPointChanged.AddUObject(VMGameScreen->HeroStatus, &UVMHeroStatusWidget::UpdateMaxHealthPoint);
			Hero->GetStatComponent()->OnCurrentHealthPointChanged.AddUObject(VMGameScreen->HeroStatus, &UVMHeroStatusWidget::UpdateCurrentHealthPoint);
			Hero->GetStatComponent()->OnManaPointChanged.AddUObject(VMGameScreen->HeroStatus, &UVMHeroStatusWidget::UpdateMaxManaPoint);
			Hero->GetStatComponent()->OnCurrentManaPointChanged.AddUObject(VMGameScreen->HeroStatus, &UVMHeroStatusWidget::UpdateCurrentManaPoint);
		}
	}

	if (VMNPCDialogueScreenClass != nullptr)
	{
		VMNPCDialogueScreen = CreateWidget<UVMNPCDialogueScreen>(this, VMNPCDialogueScreenClass);
		if (VMNPCDialogueScreen != nullptr)
		{
			VMNPCDialogueScreen->AddToViewport();
			VMNPCDialogueScreen->SetVisibility(ESlateVisibility::Hidden);
			ScreenUIMap.Add(EScreenUIType::DialogueScreen, VMNPCDialogueScreen);
		}
	}

	if (VMShopScreenClass != nullptr)
	{
		VMShopScreen = CreateWidget<UVMShopScreen>(this, VMShopScreenClass);
		if (VMShopScreen != nullptr)
		{
			VMShopScreen->AddToViewport();
			VMShopScreen->SetVisibility(ESlateVisibility::Hidden);
			ScreenUIMap.Add(EScreenUIType::ShopScreen, VMShopScreen);
		}
	}

	if (SkillsCooldownWidgetClass != nullptr)
	{
		SkillsCooldownWidget = CreateWidget<UVMSkillsCooldownWidget>(this, SkillsCooldownWidgetClass);
		if (SkillsCooldownWidget != nullptr)
		{
			SkillsCooldownWidget->AddToViewport();

			AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(GetPawn());
			Hero->GetSkillComponent()->OnBasicSkillExecute.AddUObject(SkillsCooldownWidget, &UVMSkillsCooldownWidget::StartBasicSkillCooldown);
			Hero->GetSkillComponent()->OnAdvancedSkillExecute.AddUObject(SkillsCooldownWidget, &UVMSkillsCooldownWidget::StartAdvancedSkillCooldown);
			Hero->GetSkillComponent()->OnMovementSkillExecute.AddUObject(SkillsCooldownWidget, &UVMSkillsCooldownWidget::StartMovementSkillCooldown);
			Hero->GetSkillComponent()->OnUltimateSkillExecute.AddUObject(SkillsCooldownWidget, &UVMSkillsCooldownWidget::StartUltimateSkillCooldown);
		}
	}
}


void AVMRPGPlayerController::OpenInventory()
{
	if (!InventoryPanel)
	{
		InventoryPanel = CreateWidget<UVMInventoryPanel>(this, InventoryPanelClass);
		if (InventoryPanel)
		{
			InventoryPanel->AddToViewport();
		}
	}

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	SetInputMode(InputMode);
	bShowMouseCursor = true;

	UE_LOG(LogTemp, Warning, TEXT("Inventory Opened → GameAndUI Mode"));
}

void AVMRPGPlayerController::CloseInventory()
{
	if (InventoryPanel)
	{
		InventoryPanel->RemoveFromParent();
		InventoryPanel = nullptr;
	}

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
	bShowMouseCursor = false;

	UE_LOG(LogTemp, Warning, TEXT("Inventory Closed → GameOnly Mode"));
}