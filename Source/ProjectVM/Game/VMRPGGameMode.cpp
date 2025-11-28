// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMRPGGameMode.h"
#include "Game/VMRPGPlayerController.h"
#include "Hero/Alpha/Alpha.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Character/VMCharacterHeroHUD.h"
#include "Quest/VMQuestManager.h"

AVMRPGGameMode::AVMRPGGameMode()
{
	PlayerControllerClass = AVMRPGPlayerController::StaticClass();

	ConstructorHelpers::FClassFinder<AHUD> HUDClassRef(TEXT("/Script/Engine.Blueprint'/Game/Project/HUD/BP_VMCharacterHeroHUD.BP_VMCharacterHeroHUD_C'"));
	if (HUDClassRef.Class)
	{
		HUDClass = HUDClassRef.Class;
	}
	DefaultPawnClass = AAlpha::StaticClass();
}

void AVMRPGGameMode::StartPlay()
{
	Super::StartPlay();

	GetGameInstance()->GetSubsystem<UVMQuestManager>()->AssignQuestToNPC(TEXT("Main01"));
	
	AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Hero->OnHeroDeath.AddUObject(this, &AVMRPGGameMode::GameOver);
}

void AVMRPGGameMode::GameOver()
{
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->ShowGameOverUI();
	}

	FTimerHandle RestartTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RestartTimerHandle, this, &AVMRPGGameMode::ResurrectHero, 5.0f, false);
}

void AVMRPGGameMode::ResurrectHero()
{
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC)
	{
		PC->HideGameOverUI();
		PC->HideBossStatusWidget();
	}
	
	AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Hero->Resurrect();
}