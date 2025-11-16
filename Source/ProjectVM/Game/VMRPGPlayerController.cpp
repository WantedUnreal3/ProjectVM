// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMRPGPlayerController.h"

#include "Hero/VMCharacterHeroBase.h"
#include "Hero/VMHeroStatComponent.h"
#include "UI/Common/VMGameScreen.h"
#include "UI/Dialogue/VMNPCDialogueScreen.h"
#include "UI/Shop/VMShopScreen.h"
#include "UI/Stat/VMHeroStatusWidget.h"

AVMRPGPlayerController::AVMRPGPlayerController()
{
	//게임 스크린 로드
	static ConstructorHelpers::FClassFinder<UVMGameScreen> VMGameScreenRef(TEXT("/Game/Project/UI/WBP_VMGameScreen.WBP_VMGameScreen_C"));
	if (VMGameScreenRef.Succeeded())
	{
		VMGameScreenClass = VMGameScreenRef.Class;
	}

	//대화 스크린 로드
	static ConstructorHelpers::FClassFinder<UVMNPCDialogueScreen> VMNPCDialogueScreenRef(TEXT("/Game/Project/UI/WBP_VMNPCDialogueScreen.WBP_VMNPCDialogueScreen_C"));

	if (VMNPCDialogueScreenRef.Succeeded())
	{
		VMNPCDialogueScreenClass = VMNPCDialogueScreenRef.Class;
	}

	//상점 스크린 로드
	static ConstructorHelpers::FClassFinder<UVMShopScreen> VMShopScreenRef(TEXT("/Game/Project/UI/WBP_VMShopScreen.WBP_VMShopScreen_C"));

	if (VMShopScreenRef.Succeeded())
	{
		VMShopScreenClass = VMShopScreenRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UVMHeroStatusWidget> VMHeroStatusWidgetRef(TEXT("/Game/Project/UI/Stat/WBP_HeroStatus.WBP_HeroStatus_C"));
	if (VMHeroStatusWidgetRef.Succeeded())
	{
		VMHeroStatusWidgetClass = VMHeroStatusWidgetRef.Class;
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
			VMGameScreen->SetVisibility(ESlateVisibility::Visible);
			ScreenUIMap.Add(EScreenUIType::GameScreen, VMGameScreen);
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

	if (VMHeroStatusWidgetClass != nullptr)
	{
		VMHeroStatusWidget = CreateWidget<UVMHeroStatusWidget>(this, VMHeroStatusWidgetClass);
		if (VMHeroStatusWidget != nullptr)
		{
			VMHeroStatusWidget->AddToViewport();
			AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(GetPawn());
			Hero->GetStatComponent()->OnHealthPointPercentageChanged.AddUObject(VMHeroStatusWidget, &UVMHeroStatusWidget::RefreshHealthPointBar);
			Hero->GetStatComponent()->OnManaPointPercentageChanged.AddUObject(VMHeroStatusWidget, &UVMHeroStatusWidget::RefreshManaPointBar);
		}
	}
}
