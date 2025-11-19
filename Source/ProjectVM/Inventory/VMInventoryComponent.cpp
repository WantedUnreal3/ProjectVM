// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/VMInventoryComponent.h"
#include "Item/Equipment/VMEquipment.h"
#include "Inventory/VMPickup.h"

// Sets default values for this component's properties
UVMInventoryComponent::UVMInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UVMInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

UVMEquipment* UVMInventoryComponent::FindMatchingItem(UVMEquipment* ItemIn) const
{
	if (ItemIn)
	{
		if (InventoryContents.Contains(ItemIn))
		{
			return ItemIn;
		}
	}
	return nullptr;
}

UVMEquipment* UVMInventoryComponent::FindNextItemByID(UVMEquipment* ItemIn) const
{
	if (ItemIn)
	{
		if (const TArray<TObjectPtr<UVMEquipment>>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
		{
			return *Result;
		}
	}
	return nullptr;
}

UVMEquipment* UVMInventoryComponent::FindNextPartialStack(UVMEquipment* ItemIn) const
{
	return TObjectPtr<UVMEquipment>();
}

FItemAddResult UVMInventoryComponent::HandleAddItem(UVMEquipment* InputItem)
{
	if (GetOwner())
	{
		// handle nonstackable

		if (!InputItem->bIsStackable)
		{
			return HandleNonStackableItems(InputItem);
		}
		
	}
	check(false);
	return FItemAddResult::AddedNone(FText::FromString("Try Add Item fallthrough error. GetOwner() check somehow failed."));
}

void UVMInventoryComponent::RemoveSingleInstanceOfItem(UVMEquipment* ItemToRemove)
{ 
	InventoryContents.RemoveSingle(ItemToRemove);
	OnInventoryUpdated.Broadcast();
}

int32 UVMInventoryComponent::RemoveAmountOfItem(UVMEquipment* ItemIn, int32 DesiredAmountToRemove)
{
	return int32();
}

void UVMInventoryComponent::SplitExistingStack(UVMEquipment* ItemIn, const int32 AmountToSplit)
{
	if (!(InventoryContents.Num() + 1 > SlotCapacity))
	{
		RemoveAmountOfItem(ItemIn, AmountToSplit);
		AddNewItem(ItemIn, AmountToSplit);
	}
}

void UVMInventoryComponent::RemoveItem(UVMEquipment* Item)
{
	if (!Item)
		return;

	int32 Index = InventoryContents.Find(Item);
	if (Index != INDEX_NONE)
	{
		InventoryContents.RemoveAt(Index);

		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Removed %s from inventory"),
			*Item->GetEquipmentInfo().ItemName);
	}
}


FItemAddResult UVMInventoryComponent::HandleNonStackableItems(UVMEquipment* InputItem)
{
	if (!InputItem)
	{
		return FItemAddResult::AddedNone(FText::FromString(TEXT("Invalid item.")));
	}

	AddNewItem(InputItem, 1);

	const FVMEquipmentInfo& Info = InputItem->GetEquipmentInfo();

	const FText Msg = FText::Format(
		FText::FromString(TEXT("Successfully added {0} to the inventory.")),
		FText::FromString(Info.ItemName)
	);

	return FItemAddResult::AddedAll(1, Msg);
}

void UVMInventoryComponent::AddNewItem(UVMEquipment* Item, const int32 AmountToAdd)
{
	UVMEquipment* NewItem;

	if (Item->bIsCopy || Item->bIsPickup)
	{
		// if the item is already a copy, or is a world pickup
		NewItem = Item;
		NewItem->ResetItemFlags();
	}
	else
	{
		// used when splititing or gragging to/from another inventory
		NewItem = Item->CreateItemCopy();
	}

	NewItem->OwningInventory = this;

	InventoryContents.Add(NewItem);
	//InventoryTotalWeight += NewItem->GetItemStackWeight();
	OnInventoryUpdated.Broadcast();
}




// Called every frame
//void UVMInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

