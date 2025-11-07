#include "Quest/VMQuestManager.h"
#include "Core/VMLoadManager.h"

void UVMQuestManager::AssignQuestToNPC(FName QuestId)
{
	FVMQuestData* NowQuestData = GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetQuestDataRow(QuestId);
	if (NowQuestData != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Get Quest : %s"), *NowQuestData->QuestTitle.ToString());
	}
}
