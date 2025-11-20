#include "Quest/VMQuestManager.h"
#include "Core/VMLoadManager.h"
#include "NPC/VMNPC.h"
#include "Game/VMRPGPlayerController.h"
#include "UI/Common/VMGameScreen.h"
#include "UI/Quest/VMQuestDataObject.h"
#include "UI/Quest/VMQuestTracker.h"
#include "UI/Quest/VMQuestTrackerWidget.h"
#include "Components/ListView.h"
#include "Hero/VMCharacterHeroBase.h"

void UVMQuestManager::AssignQuestToNPC(FName QuestId)
{
	FVMQuestData* NowQuestData = GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetQuestDataRow(QuestId);
	if (NowQuestData != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Quest : %s"), *NowQuestData->QuestTitle.ToString());
		// 퀘스트 데이터 전달 (델리게이트 브로드캐스트)
		OnQuestPublished.Broadcast(*NowQuestData);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest Not Found: %s"), *QuestId.ToString());
	}
}

void UVMQuestManager::AcceptQuest(FVMQuestData QuestData)
{
	UE_LOG(LogTemp, Log, TEXT("퀘스트 수락"));
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}

	UVMGameScreen* GameScreen = Cast<UVMGameScreen>(PC->GetScreen(EScreenUIType::GameScreen));
	if (GameScreen == nullptr)
	{
		return;
	}

	UVMQuestDataObject* NewQuestDataObject = NewObject<UVMQuestDataObject>(this);
	NewQuestDataObject->QuestData = QuestData;
	GameScreen->GameScreenQuestTracker->QuestTrackerListView->AddItem(NewQuestDataObject);


	//타겟에 해당하는 TArray가 있는지 확인하고 데이터 추가 
	if (!CurrentQuests.Contains(QuestData.QuestTarget))
	{
		CurrentQuests.Add(QuestData.QuestTarget, TArray<UVMQuestDataObject*>());
	}

	CurrentQuests.Find(QuestData.QuestTarget)->Add(NewQuestDataObject);
}

void UVMQuestManager::ClearQuest(FVMQuestData QuestData)
{
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}

	UVMGameScreen* GameScreen = Cast<UVMGameScreen>(PC->GetScreen(EScreenUIType::GameScreen));
	if (GameScreen == nullptr)
	{
		return;
	}
	UListView* QuestListView = GameScreen->GameScreenQuestTracker->QuestTrackerListView;
	if (QuestListView == nullptr)
	{
		return;
	}

	//퀘스트 리스트뷰에서 퀘스트 삭제
	for (UObject* Item : QuestListView->GetListItems())
	{
		UVMQuestDataObject* QuestObj = Cast<UVMQuestDataObject>(Item);
		if (QuestObj && QuestObj->QuestData.QuestId == QuestData.QuestId)
		{
			QuestListView->RemoveItem(QuestObj);
			break;
		}
	}

	//다음 퀘스트 NPC한테 전달
	TArray<FString> NextQuests;
	QuestData.NextQuestId.ParseIntoArray(NextQuests, TEXT(","), true);
	for (const FString NextQuest : NextQuests)
	{
		AssignQuestToNPC(FName(NextQuest));
	}

	//Reward
	if (QuestData.Reward == FName("Money"))
	{
		AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(PC->GetPawn());
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("AVMCharacterHeroBase is nullptr"));
			return;
		}

		Player->GetInventory()->AddMoney(QuestData.RewardCount);
	}

}

void UVMQuestManager::CompleteQuestForNPC(FVMQuestData QuestData)
{
	OnQuestCompleted.Broadcast(QuestData);
}

void UVMQuestManager::NotifyMonsterDeath(EMonsterName MonsterType)
{
	UE_LOG(LogTemp, Log, TEXT("몬스터 죽음 : %s"), *UEnum::GetValueAsString(MonsterType));

	FName Target;

	// TODO: 상윤님 CSV 변경 부분
	switch (MonsterType)
	{
	case EMonsterName::Warrior:
		Target = "Warrior";
		break;
	case EMonsterName::Wizard:
		Target = "Wizard";
		break;
	case EMonsterName::Archer:
		Target = "Archer";
		break;
	case EMonsterName::MinionMelee:
		Target = "MinionMelee";
		break;
	case EMonsterName::MinionRanged:
		Target = "MinionRanged";
		break;
	case EMonsterName::MinionSiege:
		Target = "MinionSiege";
		break;
	case EMonsterName::MinionSuper:
		Target = "MinionSuper";
		break;
	case EMonsterName::Boss:
		Target = "Boss";
		break;
	default:
		Target = "None";
		break;
	}

	TArray<UVMQuestDataObject*>* Quests = CurrentQuests.Find(Target);
	if (Quests != nullptr)
	{
		UpdateQuestProgress(*Quests);
	}
}

void UVMQuestManager::NotifyItemCollecting()
{
	UE_LOG(LogTemp, Log, TEXT("QuestManager: 아이템 수집"));

	TArray<UVMQuestDataObject*>* Quests = CurrentQuests.Find(FName("ItemCollect"));
	if (Quests != nullptr)
	{
		UpdateQuestProgress(*Quests);
	}
}

void UVMQuestManager::UpdateQuestProgress(TArray<UVMQuestDataObject*> Quests)
{
	UE_LOG(LogTemp, Log, TEXT("퀘스트 카운터 증가"));
	for (UVMQuestDataObject* Quest : Quests)
	{
		if (Quest == nullptr)  // nullptr 검사
		{
			UE_LOG(LogTemp, Warning, TEXT("QuestDataObject is nullptr!"));
			continue;
		}

		if (Quest->CurrentCount < Quest->QuestData.TargetCount)
		{
			Quest->CurrentCount++;
			Quest->SetQuestTrackerText();

			if (Quest->CurrentCount == Quest->QuestData.TargetCount)
			{
				CompleteQuestForNPC(Quest->QuestData);
			}
		}
	}
}
