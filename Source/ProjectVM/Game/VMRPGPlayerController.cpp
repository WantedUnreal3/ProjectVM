// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMRPGPlayerController.h"
#include "UI/Common/VMGameScreen.h"
#include "UI/Dialogue/VMNPCDialogueScreen.h"

AVMRPGPlayerController::AVMRPGPlayerController()
{
	static ConstructorHelpers::FClassFinder<UVMGameScreen> VMGameScreenRef(TEXT("/Game/Project/UI/WBP_VMGameScreen.WBP_VMGameScreen_C"));
	if (VMGameScreenRef.Succeeded())
	{
		VMGameScreenClass = VMGameScreenRef.Class;
	}

	//대화 위젯 클래스 로드
	static ConstructorHelpers::FClassFinder<UVMNPCDialogueScreen> VMNPCDialogueRef(TEXT("/Game/Project/UI/WBP_VMNPCDialogueScreen.WBP_VMNPCDialogueScreen_C"));

	if (VMNPCDialogueRef.Succeeded())
	{
		VMNPCDialogueClass = VMNPCDialogueRef.Class;
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

	if (VMNPCDialogueClass != nullptr)
	{
		VMNPCDialogueScreen = CreateWidget<UVMNPCDialogueScreen>(this, VMNPCDialogueClass);
		if (VMNPCDialogueScreen != nullptr)
		{
			VMNPCDialogueScreen->AddToViewport();
			VMNPCDialogueScreen->SetVisibility(ESlateVisibility::Hidden);
			ScreenUIMap.Add(EScreenUIType::DialogueScreen, VMNPCDialogueScreen);
		}
	}
}
