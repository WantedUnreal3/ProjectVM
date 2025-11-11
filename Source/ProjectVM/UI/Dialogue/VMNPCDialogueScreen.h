// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "VMNPCDialogueScreen.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMNPCDialogueScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	//bool NextText();
	FORCEINLINE void SetNPCName(const FText& NewName)
	{
		NPCName->SetText(NewName);
	}

	//FORCEINLINE void AddDialogueTalkDatas(const FText& NewTalkData)
	//{
	//	DialogueTexts.Add(NewText);
	//};

	FORCEINLINE void SetNPCText(const FText& NewText)
	{
		NPCText->SetText(NewText);
	}

	virtual void NativePreConstruct() override;

protected:
	//TArray<FText> DialogueTexts;
	//int32 CurrentDialogueIndex = 0;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NPCText;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NPCName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> DialogueOptionList;
	
};
