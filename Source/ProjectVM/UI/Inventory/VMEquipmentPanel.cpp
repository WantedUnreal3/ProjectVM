// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMEquipmentPanel.h"


#include "UI/Inventory/VMInventoryItemSlot.h"
#include "UI/Inventory/VMInventoryPanel.h"
#include "Item/Equipment/VMEquipment.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Character/VMCharacterHeroHUD.h"
#include "Hero/VMHeroStatComponent.h"

#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"


void UVMEquipmentPanel::NativeConstruct()
{
    Super::NativeConstruct();

    // 여기에 패널 초기화 코드 넣기
    // 예시:
    // EnsureSlotsInitialized();
    // RefreshEquipment(CurrentHero);
}

void UVMEquipmentPanel::RefreshEquipment(AVMCharacterHeroBase* InHero)
{
    // 최소한 이렇게라도 구현이 있어야 링크 오류가 안 난다.
    if (!InHero)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel::RefreshEquipment - InHero is null"));
        return;
    }
}
void UVMEquipmentPanel::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    // 필요하면 시작 시 바로 슬롯 만들어두기
    EnsureSlotsInitialized();
}

void UVMEquipmentPanel::EnsureSlotsInitialized()
{

     // 이미 만들어져 있으면 다시 만들 필요 없음
    if (WeaponSlots.Num() > 0)
        return;

    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel::EnsureSlotsInitialized CALLED"));

    if (!SlotWrapBox)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("EnsureSlotsInitialized: SlotWrapBox is NULL"));
        return;
    }

    if (!ItemBoxClass)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("EnsureSlotsInitialized: ItemBoxClass is NULL"));
        return;
    }

    // 기존 자식/배열 초기화
    SlotWrapBox->ClearChildren();
    WeaponSlots.Empty();

    // NumEquipmentSlots 만큼 장비 슬롯 생성
    for (int32 i = 0; i < NumEquipmentSlots; ++i)
    {
        UVMInventoryItemSlot* NewSlotWidget =
            CreateWidget<UVMInventoryItemSlot>(this, ItemBoxClass);

        if (NewSlotWidget)
        {
            // 장비창에서는 Tooltip 끄기
            NewSlotWidget->bEnableTooltip = false;

            SlotWrapBox->AddChildToWrapBox(NewSlotWidget);
        }

        UE_LOG(LogTemp, Warning,
            TEXT("EnsureSlotsInitialized: CreateWidget idx=%d -> %s"),
            i, *GetNameSafe(NewSlotWidget));

        if (!NewSlotWidget)
            continue;

        //이 슬롯은 '장비 슬롯'이라는 표시
        NewSlotWidget->SlotType = ESlotType::Equipment;
        NewSlotWidget->EquipmentPanelRef = this;

        // WrapBox에 추가
        SlotWrapBox->AddChildToWrapBox(NewSlotWidget);
        WeaponSlots.Add(NewSlotWidget);

        // 아직 장착된 아이템 없음
        NewSlotWidget->SetItemReference(nullptr);
    }

    UE_LOG(LogTemp, Warning,
        TEXT("EnsureSlotsInitialized DONE, WeaponSlots.Num=%d"),
        WeaponSlots.Num());
}

int32 UVMEquipmentPanel::TryEquipToEmptySlot(UVMEquipment* Item)
{
    UE_LOG(LogTemp, Warning,
        TEXT("EquipmentPanel::TryEquipToEmptySlot called. WeaponSlots.Num=%d, Item=%s"),
        WeaponSlots.Num(),
        *GetNameSafe(Item));

    if (!Item)
        return INDEX_NONE;

    // 슬롯이 아직 안 만들어졌다면 여기서라도 생성
    EnsureSlotsInitialized();

    for (int32 i = 0; i < WeaponSlots.Num(); ++i)
    {
        UVMInventoryItemSlot* EquipSlot = WeaponSlots[i];
        if (!EquipSlot)
            continue;

        UE_LOG(LogTemp, Warning,
            TEXT("EquipmentPanel: Check Slot %d: Widget=%s ItemRef=%p"),
            i,
            *GetNameSafe(EquipSlot),
            EquipSlot->GetItemReference());

        if (EquipSlot->GetItemReference() == nullptr)
        {
            UE_LOG(LogTemp, Warning,
                TEXT("EquipmentPanel: EMPTY SLOT FOUND -> %d"), i);

            // 인벤토리 바인딩 제거
            EquipSlot->OnItemDoubleClicked.Clear();

            // 장비 패널용 바인딩 추가
            EquipSlot->OnItemDoubleClicked.AddDynamic(
                this, &UVMEquipmentPanel::HandleEquipmentSlotDoubleClicked);

            // 인벤토리랑 동일하게 아이콘/텍스트 세팅
            EquipSlot->SetItemReference(Item);


            return i;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: No free equipment slot"));
    return INDEX_NONE;
}

void UVMEquipmentPanel::ClearAllSlots()
{
    EnsureSlotsInitialized();

    for (UVMInventoryItemSlot* EquipSlot : WeaponSlots)
    {
        if (EquipSlot)
        {
            EquipSlot->SetItemReference(nullptr);
        }
    }
}

void UVMEquipmentPanel::HandleEquipmentSlotDoubleClicked(UVMEquipment* Item)
{
    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel::HandleEquipmentSlotDoubleClicked CALLED, Item=%s"), Item ? *Item->GetEquipmentInfo().ItemName : TEXT("NULL"));
    
    if (!Item)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: DoubleClick but Item is NULL"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: DoubleClicked %s"),
        *Item->GetEquipmentInfo().ItemName);

    // 1) PlayerController / Hero 가져오기
    APlayerController* PC = GetOwningPlayer();
    if (!PC) return;

    AVMCharacterHeroBase* Hero = Cast<AVMCharacterHeroBase>(PC->GetPawn());
    if (!Hero) return;

    // 2) 캐릭터에서 장비 해제 (스탯 제거 포함)
    Hero->GetStatComponent()->RemoveEquipmentStats(Item);

    // 3) Hero의 인벤토리 가져오기
    UVMInventoryComponent* Inventory = Hero->GetInventory();
    if (Inventory)
    {
        Inventory->AddNewItem(Item, 1);   // 인벤토리로 돌려보내기
    }

    // 4) 장비 슬롯 UI 비우기
    for (UVMInventoryItemSlot* EquipSlot : WeaponSlots)
    {
        if (EquipSlot && EquipSlot->GetItemReference() == Item)
        {
            EquipSlot->ClearItem();
            break;
        }
    }

    // 5) 인벤토리 UI 새로고침
    APlayerController* LocalPC = Hero->GetController<APlayerController>();
    if (LocalPC)
    {
        if (AVMCharacterHeroHUD* HUD = Cast<AVMCharacterHeroHUD>(LocalPC->GetHUD()))
        {
            if (HUD->InventoryPanel)
            {
                HUD->InventoryPanel->RefreshInventory();
            }
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("EquipmentPanel: Unequipped %s & returned to inventory"),
        *Item->GetEquipmentInfo().ItemName);
}
