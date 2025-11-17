// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/WrapBox.h"
#include "Components/TextBlock.h"
#include "Inventory/VMInventoryComponent.h"

#include "VMInventoryPanel.generated.h"


class UVMInventoryItemSlot;
class UVMInventoryComponent;
class AVMCharacterHeroBase;


/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMInventoryPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void RefreshInventory();


	UPROPERTY(meta = (BindWidget))
	UWrapBox* InventoryWrapBox;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UTextBlock> WeightInfoText;
	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UTextBlock> WeightInfo;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UTextBlock> CapacityInfoText;

	//UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UTextBlock> CapacityInfo;

	UPROPERTY()
	AVMCharacterHeroBase* Character;

	UPROPERTY()
	UVMInventoryComponent* InventoryReference;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UVMInventoryItemSlot> InventorySlotClass;

protected:
	void SetInfoText() const;
	virtual void NativeOnInitialized() override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
};
