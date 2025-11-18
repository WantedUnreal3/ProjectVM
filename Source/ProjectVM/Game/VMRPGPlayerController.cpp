// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMRPGPlayerController.h"

#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"
#include "UI/Common/VMGameScreen.h"
#include "UI/Dialogue/VMNPCDialogueScreen.h"
#include "UI/Shop/VMShopScreen.h"
#include "UI/Stat/VMHeroStatusWidget.h"
#include "UI/Common/VMInteractKeyWidget.h"

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
			Hero->GetStatComponent()->OnHealthPointPercentageChanged.AddUObject(VMGameScreen->HeroStatus, &UVMHeroStatusWidget::RefreshHealthPointBar);
			Hero->GetStatComponent()->OnManaPointPercentageChanged.AddUObject(VMGameScreen->HeroStatus, &UVMHeroStatusWidget::RefreshManaPointBar);
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
}
