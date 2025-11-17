// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMInventoryTooltip.generated.h"

class UVMInventoryItemSlot;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMInventoryTooltip : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	//UInventoryItemSlot* InventorySlotBeingHovered;
	TObjectPtr<UVMInventoryItemSlot> InventorySlotBeingHovered;

	UPROPERTY(meta = (BindWidget))
	//TObjectPtr<UTextBlock> ItemName;
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemID;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemDesc;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemLevel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DefensivePower;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthPoint;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ManaPoint;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ManaRegeneration;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Ability;

protected:
	virtual void NativeConstruct() override;
	
};
