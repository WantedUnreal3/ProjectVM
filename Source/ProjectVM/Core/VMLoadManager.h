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

	////모든 데이터 로드해서 맵으로 반환하는 템플릿 함수
	//template<typename T>
	//TMap<FName, T> GetTableMap(UDataTable*& Table, const TCHAR* TablePath, const TCHAR* TableName)
	//{
	//	TMap<FName, T> Result;
	//	// 테이블이 로드되지 않은 경우 로드 시도
	//	if (Table == nullptr)
	//	{
	//		Table = LoadObject<UDataTable>(nullptr, TablePath);
	//		if (Table != nullptr)
	//		{
	//			UE_LOG(LogTemp, Log, TEXT("%s Loaded Successfully"), TableName);
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Error, TEXT("Failed to Load %s"), TableName);
	//			return Result;
	//		}
	//	}

	//	// 모든 Row 가져오기
	//	static const FString ContextString(TEXT("GetTableMap"));
	//	for (const auto& Pair : Table->GetRowMap())
	//	{
	//		FName RowName = Pair.Key;
	//		T* RowData = reinterpret_cast<T*>(Pair.Value);

	//		if (RowData)
	//		{
	//			Result.Add(RowName, *RowData); // 구조체 복사


	//		}
	//	}

	//	UE_LOG(LogTemp, Log, TEXT("%s table loaded with %d rows"), TableName, Result.Num());
	//	return Result;
	//}

private:
	class UDataTable* NPCTalkDataTable;
	class UDataTable* NPCDataTable;
	class UDataTable* QuestDataTable;

	//데이터	테이블 경로
	FString NPCTalkTablePath = TEXT("/Game/Project/GameData/VMNPCTalkDataTable.VMNPCTalkDataTable");
	FString NPCDataTablePath = TEXT("/Game/Project/GameData/VMNPCDataTable.VMNPCDataTable");
	FString QuestDataTablePath = TEXT("/Game/Project/GameData/VMQuestDataTable.VMQuestDataTable");
};
