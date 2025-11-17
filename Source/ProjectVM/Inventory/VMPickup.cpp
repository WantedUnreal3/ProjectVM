// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/VMPickup.h"
#include "Inventory/VMInventoryComponent.h"

#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"

// Sets default values
AVMPickup::AVMPickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AVMPickup::InitializePickup(const TSubclassOf<UVMEquipment> PickupClass)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FVMEquipmentInfo* ItemData = ItemDataTable->FindRow<FVMEquipmentInfo>(DesiredItemID, DesiredItemID.ToString());

		ItemReference = NewObject<UVMEquipment>(this, PickupClass);

		ItemReference->GetEquipmentInfo();
	/*	ItemReference->ItemType = ItemData->ItemType;
		ItemReference->ItemQuality = ItemData->ItemQuality;
		ItemReference->ItemNumericData = ItemData->ItemNumericData;
		ItemReference->ItemTextData = ItemData->ItemTextData;
		ItemReference->ItemAssetData = ItemData->ItemAssetData;*/

		ItemReference->SetQuantity(1);

		//PickupMesh->SetStaticMesh(ItemData->ItemAssetData.Mesh);

		UpdateInteractableData();

	}
}

void AVMPickup::InitializeDrop(UVMEquipment* ItemDrop)
{

	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

	ItemReference = ItemDrop;
	ItemReference->SetQuantity(1);
	
	InstanceInteractableData.Name = FText::FromString(Info.ItemName);
	InstanceInteractableData.Action = FText::FromString(TEXT("획득"));
	/*InstanceInteractableData.Quantity = ItemReference->Quantity;*/
	UpdateInteractableData();
}

void AVMPickup::BeginFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(true);
	}
}

void AVMPickup::EndFocus()
{
	if (PickupMesh)
	{
		PickupMesh->SetRenderCustomDepth(false);
	}
}

// Called when the game starts or when spawned
void AVMPickup::BeginPlay()
{
	Super::BeginPlay();
	
}

void AVMPickup::BeingInteract(AVMCharacterHeroBase* Character)
{
	if (Character)
	{
		TakePickup(Character);
	}
}

void AVMPickup::UpdateInteractableData()
{
	InstanceInteractableData.InteractableType = EInteractableType::Pickup;
	InteractableData = InstanceInteractableData;
}

void AVMPickup::TakePickup(const AVMCharacterHeroBase* Taker)
{
}

//// Called every frame
//void AVMPickup::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

//void AVMPickup::BeingInteract(AVMCharacterHeroBase* Character)
//{
//	if (Character)
//	{
//		TakePickup(Character);
//	}
//}