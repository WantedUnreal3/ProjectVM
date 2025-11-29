// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VMLoadManager.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMLoadManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	// 데이터 테이블에서 특정 행을 가져오는 함수
	struct FVMNPCTalkData* GetNPCTalkRow(FName RowName);
	struct FVMNPCData* GetNPCDataRow(FName RowName);
	struct FVMQuestData* GetQuestDataRow(FName RowName);
	struct FVMEquipmentInfo* GetEquipmentInfoRow(FName RowName);

	//첫 Row를 키로 하고 구조체를 값으로 하는 맵 반환 함수
	//TMap<FName, FVMQuestData> GetAllQuestDataAsMap();

private:
	// 테이블 Row로 데이터 가져오기 템플릿 함수
	// UDataTable *& Table 로 원본값 갱신
	template<typename T>
	T* GetTableRow(UDataTable*& Table, const TCHAR* TablePath, FName RowName, const TCHAR* TableName)
	{
		// 테이블이 로드되지 않은 경우 로드 시도
		if (Table == nullptr)
		{
			Table = LoadObject<UDataTable>(nullptr, TablePath);
			if (Table != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("%s Loaded Successfully"), TableName);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to Load %s"), TableName);
				return nullptr;
			}
		}

		//RowName에 해당하는 데이터 찾기
		T* RowData = Table->FindRow<T>(RowName, TableName);

		if (RowData == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Row %s not found in table %s"), *RowName.ToString(), TableName);
			return nullptr;
		}

		return RowData;
	}

private:

	UPROPERTY()
	class UDataTable* NPCTalkDataTable;
	UPROPERTY()
	class UDataTable* NPCDataTable;
	UPROPERTY()
	class UDataTable* QuestDataTable;
	UPROPERTY()
	class UDataTable* EquipmentInfoTable;

	//데이터	테이블 경로
	FString NPCTalkTablePath = TEXT("/Game/Project/GameData/VMNPCTalkDataTable.VMNPCTalkDataTable");
	FString NPCDataTablePath = TEXT("/Game/Project/GameData/VMNPCDataTable.VMNPCDataTable");
	FString QuestDataTablePath = TEXT("/Game/Project/GameData/VMQuestDataTable.VMQuestDataTable");
	FString EquipmentInfoTableTath = TEXT("/Game/Project/Item/VMEquipmentInfo.VMEquipmentInfo");
};
