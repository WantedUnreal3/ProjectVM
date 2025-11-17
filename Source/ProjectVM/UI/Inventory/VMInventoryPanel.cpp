// Fill out your copyright notice in the Description page of Project Settings.



#include "UI/Inventory/VMInventoryPanel.h"
#include "UI/Inventory/VMInventoryItemSlot.h"
#include "Inventory/VMInventoryComponent.h"
#include "Hero/VMCharacterHeroBase.h"

#include "Components/WrapBox.h"
#include "Components/TextBlock.h"



void UVMInventoryPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    Character = Cast<AVMCharacterHeroBase>(GetOwningPlayerPawn());

    if (Character)
    {
        InventoryReference = Character->GetInventory();
        if (InventoryReference)
        {
            InventoryReference->OnInventoryUpdated.AddUObject(this, &UVMInventoryPanel::RefreshInventory);
            SetInfoText();
        }
    }
}

void UVMInventoryPanel::RefreshInventory()
{
    if (InventoryReference && InventorySlotClass)
    {
        InventoryWrapBox->ClearChildren();
        for (UVMEquipment* const& InventoryItem : InventoryReference->GetInventoryContents())
        {
            UVMInventoryItemSlot* ItemSlot = CreateWidget<UVMInventoryItemSlot>(this, InventorySlotClass);
            ItemSlot->SetItemReference(InventoryItem);

            InventoryWrapBox->AddChildToWrapBox(ItemSlot);

        }

        SetInfoText();
    }
}

void UVMInventoryPanel::SetInfoText() const
{
}

bool UVMInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

}
