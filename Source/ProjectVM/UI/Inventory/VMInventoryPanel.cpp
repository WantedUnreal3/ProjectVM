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
    UE_LOG(LogTemp, Warning, TEXT("InventoryPanel::RefreshInventory called"));

    if (!InventoryReference)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryPanel::RefreshInventory - InventoryReference is NULL"));
        return;
    }

    if (!InventorySlotClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryPanel::RefreshInventory - InventorySlotClass is NULL"));
        return;
    }

    const TArray<UVMEquipment*>& Contents = InventoryReference->GetInventoryContents();
    UE_LOG(LogTemp, Warning, TEXT("InventoryPanel::RefreshInventory - NumItems: %d"), Contents.Num());

    InventoryWrapBox->ClearChildren();

    for (UVMEquipment* const& InventoryItem : Contents)
    {
        UE_LOG(LogTemp, Warning, TEXT("  Slot for item: %s"),
            InventoryItem ? *InventoryItem->GetEquipmentInfo().ItemName : TEXT("NULL ITEM"));

        UVMInventoryItemSlot* ItemSlot =
            CreateWidget<UVMInventoryItemSlot>(this, InventorySlotClass);

        ItemSlot->SetItemReference(InventoryItem);  // 여기서 SetItemReference 로그가 떠야 함

        InventoryWrapBox->AddChildToWrapBox(ItemSlot);
    }

    SetInfoText();
}

void UVMInventoryPanel::SetInfoText() const
{
}

bool UVMInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

}
