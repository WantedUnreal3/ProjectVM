// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMInventoryTooltip.h"
#include "UI/Inventory/VMInventoryItemSlot.h"
#include "Item/Equipment/VMEquipment.h"

void UVMInventoryTooltip::NativeConstruct()
{
	Super::NativeConstruct();

	const TObjectPtr<UVMEquipment> ItemBeingHovered = InventorySlotBeingHovered->GetItemReference();
}
