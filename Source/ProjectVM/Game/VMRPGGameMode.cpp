// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMRPGGameMode.h"
#include "Game/VMRPGPlayerController.h"
#include "Hero/Alpha/Alpha.h"
#include "UI/Character/VMCharacterHeroHUD.h"
#include "Quest/VMQuestManager.h"

AVMRPGGameMode::AVMRPGGameMode()
{
	PlayerControllerClass = AVMRPGPlayerController::StaticClass();
	HUDClass = AVMCharacterHeroHUD::StaticClass();
	DefaultPawnClass = AAlpha::StaticClass();
}

void AVMRPGGameMode::StartPlay()
{
	Super::StartPlay();
	GetGameInstance()->GetSubsystem<UVMQuestManager>()->AssignQuestToNPC(TEXT("Main01"));
}
