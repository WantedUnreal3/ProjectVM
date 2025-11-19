// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMEquipmentItemSlot.h"

#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/Equipment/VMEquipment.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"


void UVMEquipmentItemSlot::NativeConstruct()
{
    Super::NativeConstruct();
    UE_LOG(LogTemp, Log, TEXT("Test1"));
    ItemReference = nullptr;

    if (ItemIcon)
    {
        UE_LOG(LogTemp, Log, TEXT("Test2"));
        ItemMaterialInstance = ItemIcon->GetDynamicMaterial();
    }
    UE_LOG(LogTemp, Log, TEXT("Test3"));
    RefreshFromItem();

    //if (ItemIcon)
    //{
    //    ItemMaterialInstance = ItemIcon->GetDynamicMaterial();
    //}
    //RefreshFromItem();
}

void UVMEquipmentItemSlot::SetItem(UVMEquipment* InItem)
{
   ItemReference = InItem;

    const TCHAR* NameForLog = TEXT("NULL");
    if (ItemReference)
    {
        NameForLog = *ItemReference->GetEquipmentInfo().ItemName;
    }
    UE_LOG(LogTemp, Warning, TEXT("EquipSlot::SetItemReference called, Item: %s"), NameForLog);

    RefreshFromItem();
}

void UVMEquipmentItemSlot::ClearItem()
{
    ItemReference = nullptr;
    RefreshFromItem();
}

void UVMEquipmentItemSlot::RefreshFromItem()
{
    UE_LOG(LogTemp, Log, TEXT("Refresh1"));

    if (!ItemIcon || !ItemBorder)
        return;
    UE_LOG(LogTemp, Log, TEXT("Refresh2"));
    if (!ItemReference)
    {
        //ItemIcon->SetBrushFromTexture(nullptr);
        //ItemBorder->SetBrushColor(FLinearColor::White);
        return;
    }

    const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

    //if (Info.Icon)
    //{
    //    ItemIcon->SetBrushFromTexture(Info.Icon);
    //}
    UE_LOG(LogTemp, Log, TEXT("Refresh3 : %s"), *Info.ItemName);
    SetUp(Info);
    //else
    //{
    //    ItemIcon->SetBrushFromTexture(nullptr);
    //}
}

void UVMEquipmentItemSlot::SetUp(const FVMEquipmentInfo& Info)
{
    // 다이나믹 머터리얼 생성
    if (ItemIcon == nullptr)
    {
        UE_LOG(LogTemp, Log, TEXT("ItemIcon is nullptr"));
        return;
    }
    if (ItemIcon->GetBrush().GetResourceObject())
    {
        UMaterialInterface* BaseMat = Cast<UMaterialInterface>(ItemIcon->GetBrush().GetResourceObject());
        if (BaseMat != nullptr)
        {
            ItemMaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
            ItemIcon->SetBrushFromMaterial(ItemMaterialInstance);
        }
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("ItemIcon->GetBrush().GetResourceObject() is nullptr"));
        return;
    }
    //머터리얼 파라미터 설정
    if (ItemMaterialInstance)
    {
        ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), Info.AtlasCol);
        ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), Info.AtlasRow);
    }

}