// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VMGameMode.h"

AVMGameMode::AVMGameMode()
{
#pragma region 캐릭터 지정
	ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProjectVM.VMPlayer'"));
	ensureAlways(DefaultPawnClassRef.Class);
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}
#pragma endregion 

#pragma region 캐릭터 컨트롤러 지정
	ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/ProjectVM.VMPlayerController'"));
	ensureAlways(PlayerControllerClassRef.Class);
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}
#pragma endregion
}
