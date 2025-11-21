// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMMainMenu.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Item/Equipment/VMEquipment.h"
#include "UI/Inventory/VMInventoryPanel.h"
#include "UI/Inventory/VMEquipmentPanel.h"

void UVMMainMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}


void UVMMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    Character = Cast<AVMCharacterHeroBase>(GetOwningPlayerPawn());

    if (WBP_VMInventoryPanel && WBP_VMEquipmentPanel)
    {
        WBP_VMInventoryPanel->LinkedEquipmentPanel = WBP_VMEquipmentPanel;
        //WBP_VMEquipmentPanel->LinkedInventoryPanel = WBP_VMInventoryPanel;
    }
}

bool UVMMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return false;
}
