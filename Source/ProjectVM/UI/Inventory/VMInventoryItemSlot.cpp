// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMInventoryItemSlot.h"

void UVMInventoryItemSlot::NativeOnInitialized()
{
}

void UVMInventoryItemSlot::NativeConstruct()
{
}

FReply UVMInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
}

void UVMInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
}

void UVMInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
}

bool UVMInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return false;
}
