// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMInventoryItemSlot.generated.h"



class UVMInventoryTooltip;
class UDragItemVisual;
class UVMEquipment;
class UTextBlock;
class UBorder;
class UImage;
/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMInventoryItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetItemReference(UVMEquipment* ItemIn) { ItemReference = ItemIn; };
	FORCEINLINE UVMEquipment* GetItemReference() const { return ItemReference; };

protected:
	//UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	//TSubclassOf<UDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UVMInventoryTooltip> TooltipClass;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	UVMEquipment* ItemReference;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	UImage* ItemIcon;

	//UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	//TObjectPtr<UTextBlock> ItemQuantity;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
