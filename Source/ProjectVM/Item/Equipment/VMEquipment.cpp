// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Hero/VMCharacterHeroBase.h"

UVMEquipment::UVMEquipment()
	:bIsCopy(false), bIsPickup(false)
{
}

void UVMEquipment::ResetItemFlags()
{
	bIsCopy = false;
	bIsPickup = false;
}

TObjectPtr<UVMEquipment> UVMEquipment::CreateItemCopy() const
{
	TObjectPtr<UVMEquipment> ItemCopy = NewObject<UVMEquipment>(StaticClass());

	ItemCopy->EquipmentInfo = this->EquipmentInfo;

	return ItemCopy;
}

void UVMEquipment::SetQuantity(const int32 NewQuantity)
{
	//if (NewQuantity != Quantity)
	//{
	//	Quantity = FMath::Clamp(NewQuantity, 0, ItemNumericData.bIsStackable ? ItemNumericData.MaxStackSize : 1);

	//	if (OwningInventory)
	//	{
	//		if (Quantity <= 0)
	//		{
	//			OwningInventory->RemoveSingleInstanceOfItem(this);
	//		}
	//	}
	//}
}

void UVMEquipment::Use(AVMCharacterHeroBase* Character)
{
}

void UVMEquipment::EquipEffect(AVMCharacterHeroBase* Owner)
{

}