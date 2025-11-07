// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/VMLoadManager.h"
#include "Engine/DataTable.h"
#include "GameData/VMNPCTalkData.h"
#include "GameData/VMNPCData.h"
#include "GameData/VMQuestData.h"


FVMNPCTalkData* UVMLoadManager::GetNPCTalkRow(FName RowName)
{
	return GetTableRow<FVMNPCTalkData>(NPCTalkDataTable, *NPCTalkTablePath, RowName, TEXT("NPCTalkDataTable"));
}

FVMNPCData* UVMLoadManager::GetNPCDataRow(FName RowName)
{
	return GetTableRow<FVMNPCData>(NPCDataTable, *NPCDataTablePath, RowName, TEXT("NPCDataTable"));
}

FVMQuestData* UVMLoadManager::GetQuestDataRow(FName RowName)
{
	return GetTableRow<FVMQuestData>(QuestDataTable, *QuestDataTablePath, RowName, TEXT("QuestDataTable"));
}
