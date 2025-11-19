// Fill out your copyright notice in the Description page of Project Settings.



#include "UI/Inventory/VMInventoryPanel.h"
#include "UI/Inventory/VMInventoryItemSlot.h"
#include "UI/Inventory/VMEquipmentPanel.h"
#include "Inventory/VMInventoryComponent.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Character/VMCharacterHeroHUD.h"


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

    const TArray<UVMEquipment*>& Contents = InventoryReference->GetInventoryContents();
    UE_LOG(LogTemp, Warning, TEXT("InventoryPanel::RefreshInventory - NumItems: %d"), Contents.Num());

    InventoryWrapBox->ClearChildren();

    for (UVMEquipment* const& InventoryItem : Contents)
    {
        UE_LOG(LogTemp, Warning, TEXT("  Slot for item: %s"),
            InventoryItem ? *InventoryItem->GetEquipmentInfo().ItemName : TEXT("NULL ITEM"));

        UVMInventoryItemSlot* ItemSlot =
            CreateWidget<UVMInventoryItemSlot>(this, InventorySlotClass);

        ItemSlot->OnItemDoubleClicked.AddDynamic(this, &UVMInventoryPanel::HandleItemDoubleClicked);

        ItemSlot->SetItemReference(InventoryItem);  // 여기서 SetItemReference 로그가 떠야 함

        InventoryWrapBox->AddChildToWrapBox(ItemSlot);
    }

    SetInfoText();
}

void UVMInventoryPanel::HandleItemDoubleClicked(UVMEquipment* Item)
{
   
    UE_LOG(LogTemp, Warning, TEXT("InventoryPanel::HandleItemDoubleClicked: %s"),
        Item ? *Item->GetEquipmentInfo().ItemName : TEXT("NULL"));

    if (!Item || !InventoryReference) return;

    AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(InventoryReference->GetOwner());
    if (!Hero) return;

    // 1) 스탯 적용
    Hero->EquipFromInventory(Item);

    // 2) 장비 패널 UI
    if (APlayerController* PC = Cast<APlayerController>(Hero->GetController()))
    {
        UE_LOG(LogTemp, Warning, TEXT("InventoryPanel: PlayerController OK"));
        if (AVMCharacterHeroHUD* HUD = PC->GetHUD<AVMCharacterHeroHUD>())
        {
            UE_LOG(LogTemp, Warning, TEXT("InventoryPanel: HUD OK"));
            if (HUD->EquipmentPanel)
            {
                UE_LOG(LogTemp, Warning, TEXT("InventoryPanel: EquipmentPanel valid, calling TryEquipToEmptySlot"));

                // 수정
                const int32 EquippedIndex = HUD->EquipmentPanel->TryEquipToEmptySlot(Item);

                if (EquippedIndex != INDEX_NONE)
                {
                    // 장비칸에 장착 성공 → 인벤토리에서 제거 + 새로고침
                    InventoryReference->RemoveSingleInstanceOfItem(Item);
                    RefreshInventory();
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("InventoryPanel: No free equipment slot, item stays in inventory"));
                }
            }

        }
    }
}   

void UVMInventoryPanel::SetInfoText() const
{
}

bool UVMInventoryPanel::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

}
