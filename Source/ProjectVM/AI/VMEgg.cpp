#include "VMEgg.h"
#include "Core/VMLevelManager.h"
#include "Enemies/VMEnemyBoss.h"

AVMEgg::AVMEgg()
{
	PrimaryActorTick.bCanEverTick = false;

}

void AVMEgg::BossSpawn(FTransform SpawnTransform)
{
	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	//Spawn Level BossMap으로 한정. BossMap 없으면 퍼시스턴트 레벨에 소환
	UVMLevelManager* LevelManager = GetGameInstance()->GetSubsystem<UVMLevelManager>();
	if (LevelManager != nullptr)
	{
		ULevelStreaming* BossLevel = LevelManager->GetLevel(FName("BossMap"));
		if (BossLevel != nullptr && BossLevel->GetLoadedLevel() != nullptr)
		{
			Params.OverrideLevel = BossLevel->GetLoadedLevel();
			UE_LOG(LogTemp, Log, TEXT("Spawn location changed to BossMap"));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("BossLevel is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("LevelManager is nullptr"));
	}

	//보스 생성
	AVMEnemyBoss* SpawnedActor = World->SpawnActor<AVMEnemyBoss>(AVMEnemyBoss::StaticClass(), SpawnTransform, Params);
}
