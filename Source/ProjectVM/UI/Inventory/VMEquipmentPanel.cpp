// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMEquipmentPanel.h"
#include "UI/Inventory/VMEquipmentItemSlot.h"
#include "Inventory/VMInventoryComponent.h"

#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Item/Equipment/VMEquipment.h"

#include "Components/Widget.h"


void UVMEquipmentPanel::NativeConstruct()
{

    Super::NativeConstruct();

    WeaponSlots.Empty();
    WeaponSlots.Add(Weapon_Slot0);
    WeaponSlots.Add(Weapon_Slot1);
    WeaponSlots.Add(Weapon_Slot2);
    WeaponSlots.Add(Weapon_Slot3);
    WeaponSlots.Add(Weapon_Slot4);
    WeaponSlots.Add(Weapon_Slot5);

    // 시작은 전부 빈칸
    for (UVMEquipmentItemSlot* EquipSlot : WeaponSlots)
    {
        if (EquipSlot)
        {
            EquipSlot->SetItem(nullptr);
        }
    }
}

int32 UVMEquipmentPanel::TryEquipToEmptySlot(UVMEquipment* Item)
{

    if (!Item) return false;

    for (int32 i = 0; i < WeaponSlots.Num(); ++i)
    {
        UVMEquipmentItemSlot* EquipSlot = WeaponSlots[i];
        if (!EquipSlot)
        {
            UE_LOG(LogTemp, Warning, TEXT("Equipment Fail"));
            continue;
        }

    

        if (EquipSlot->GetItem() == nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: Equip to slot %d"), i);
            EquipSlot->SetItem(Item);
            return true;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: No free equipment slot"));
    return false;

}
