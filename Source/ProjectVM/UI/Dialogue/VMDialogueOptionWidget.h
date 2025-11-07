// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMDialogueOption.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "VMDialogueOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMDialogueOptionWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	// 블루프린트에서 바인딩
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* OptionTextBlock;

	UPROPERTY(meta = (BindWidget))
	class UButton* OptionButton;



	//위젯이 처음 생성될 때 딱 한 번 호출됨
	virtual void NativeConstruct() override;

	//리스트뷰가 데이터를 연결할 때 마다 호출됨
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;



private:
	UFUNCTION()
	void OnOptionButtonClicked();

	UPROPERTY()
	class UVMDialogueOption* Option;
};
