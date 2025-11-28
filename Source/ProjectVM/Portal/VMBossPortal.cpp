// Fill out your copyright notice in the Description page of Project Settings.


#include "Portal/VMBossPortal.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/LevelStreaming.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Core/VMLevelManager.h"

AVMBossPortal::AVMBossPortal()
{
	MapTeleportPos = FVector(12000.f, 12000.f, 0.f);
	PlayerTeleportPos = FVector(-1800.f, -3000.f, 500.f);
}

void AVMBossPortal::Interact()
{
	Super::Interact();

	LoadMap();
}

void AVMBossPortal::LoadMap()
{
	////포탈로 이동하는 이펙트 라인 숨기기
	//if (EffectLineNiagaraSystem == nullptr)
	//{
	//	return;
	//}
	//EffectLineNiagaraSystem->Deactivate(); // 비활성화

	//맵 이동
	if (!GetWorld()) return;

	UVMLevelManager* LevelManager = GetGameInstance()->GetSubsystem<UVMLevelManager>();

	if (LevelManager == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Level Manager is nullptr"));
		return;
	}

	ULevelStreaming* LoadLevel = LevelManager->GetLevel(TargetLevelName);
	//이미 로드된 레벨일 경우 재사용
	if (LoadLevel != nullptr)
	{
		LoadLevel->SetShouldBeLoaded(true);
		LoadLevel->SetShouldBeVisible(true);
		TeleportPlayerToMap(); //플레이어 맵으로 순간 이동
		UE_LOG(LogTemp, Log, TEXT("이미 로드된 맵입니다."));
		return;
	}

	//로드된 맵이 없으면 새로 로드

	bool bSuccess = false;
	// ULevelStreamingDynamic::LoadLevelInstance()는 UObject로 생성된 Streaming Level 인스턴스를 반환한다.
	// ULevelStreamingDynamic은 ULevelStreaming의 자식이다.
	ULevelStreamingDynamic* NewLevel = ULevelStreamingDynamic::LoadLevelInstance(
		GetWorld(),
		TargetLevelPath,		// FString 레벨 경로
		MapTeleportPos,			// 위치
		FRotator::ZeroRotator,	// 회전
		bSuccess				// 로드 성공 여부
	);

	if (bSuccess && NewLevel!=nullptr)
	{
		LevelManager->AddLevel(TargetLevelName, NewLevel);
		NewLevel->SetShouldBeLoaded(true);
		NewLevel->SetShouldBeVisible(true);
		UE_LOG(LogTemp, Log, TEXT("새로 맵을 로드합니다."));
		NewLevel->OnLevelLoaded.AddDynamic(this, &AVMPortal::TeleportPlayerToMap); //레벨이 로드가 완료되면 실행하는 함수 바인딩
	}
}

