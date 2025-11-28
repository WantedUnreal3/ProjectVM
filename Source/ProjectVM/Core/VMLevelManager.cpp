
#include "VMLevelManager.h"

void UVMLevelManager::AddLevel(FName NewLevelName, TObjectPtr<class ULevelStreaming> NewLevel)
{
	if (NewLevel == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("New Level is nullptr"));
		return;
	}

	LoadedLevels.Add(NewLevelName, NewLevel);
}

void UVMLevelManager::DeleteLevel(FName DeleteLevelName)
{
	TObjectPtr<class ULevelStreaming> Level = GetLevel(DeleteLevelName);

	if (Level == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("지울 레벨이 레벨 매니저에 등록되어 있지 않습니다.: %s"), *DeleteLevelName.ToString());
		return;
	}

	// 레벨 언로드
	Level->SetShouldBeVisible(false);
	Level->SetShouldBeLoaded(false);
	//TMap의 Remove는 삭제한 갯수를 리턴함.
	if (LoadedLevels.Remove(DeleteLevelName) > 0)
	{
		UE_LOG(LogTemp, Log, TEXT("레벨 삭제에 성공했습니다.: %s"), *DeleteLevelName.ToString());
	}
}
