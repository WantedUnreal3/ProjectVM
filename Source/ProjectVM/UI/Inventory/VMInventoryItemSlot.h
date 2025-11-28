// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Equipment/VMEquipment.h"
#include "VMInventoryItemSlot.generated.h"



class UVMInventoryTooltip;
class UVMDragItemVisual;
class UVMEquipment;
class UVMEquipmentInfo;
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
	FORCEINLINE UVMEquipment* GetItemReference() const { return ItemReference; };
	void SetItemReference(UVMEquipment* ItemIn);

public:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UVMDragItemVisual> DragItemVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory Slot")
	TSubclassOf<UVMInventoryTooltip> TooltipClass;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot")
	TObjectPtr<UVMEquipment> ItemReference;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;

	UPROPERTY()
	TObjectPtr<class UMaterialInstanceDynamic> ItemMaterialInstance;

	void SetUp(const FVMEquipmentInfo& Info);
	//UPROPERTY(VisibleAnywhere, Category = "Inventory Slot", meta = (BindWidget))
	//TObjectPtr<UTextBlock> ItemQuantity;

	void RefreshFromItem();

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
