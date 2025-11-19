// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/VMPickup.h"
#include "Inventory/VMInventoryComponent.h"
#include "UI/Inventory/VMInventoryPanel.h"

#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"

// Sets default values
AVMPickup::AVMPickup()
{
 
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetSimulatePhysics(true);
	SetRootComponent(PickupMesh);

}

void AVMPickup::InitializePickup(const TSubclassOf<UVMEquipment> PickupClass)
{
	if (ItemDataTable && !DesiredItemID.IsNone())
	{
		const FVMEquipmentInfo* ItemData =
			ItemDataTable->FindRow<FVMEquipmentInfo>(DesiredItemID, DesiredItemID.ToString());

		if (!ItemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("InitializePickup: Row %s NOT FOUND"),
				*DesiredItemID.ToString());
			return;
		}

		// 여기에서 DataTable 아이콘부터 확인
		UE_LOG(LogTemp, Warning, TEXT("InitializePickup: Row %s Icon:%s"),
			*ItemData->ItemName,
			ItemData->Icon ? *ItemData->Icon->GetName() : TEXT("NULL"));

		ItemReference = NewObject<UVMEquipment>(this, PickupClass);
		if (!ItemReference)
		{
			UE_LOG(LogTemp, Warning, TEXT("InitializePickup: Failed to create UVMEquipment"));
			return;
		}

		ItemReference->SetEquipmentInfo(*ItemData);   // 구조체 통째로 복사
		ItemReference->SetQuantity(1);

		InstanceInteractableData.Name = FText::FromString(ItemData->ItemName);
		InstanceInteractableData.Action = FText::FromString(TEXT("획득"));
		UpdateInteractableData();

		if (PickupMesh && ItemData->Mesh)
		{
			PickupMesh->SetStaticMesh(ItemData->Mesh);
		}
	}
}




void AVMPickup::InitializeDrop(UVMEquipment* ItemDrop)
{

	//const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

	//ItemReference = ItemDrop;
	//ItemReference->SetQuantity(1);
	//
	//InstanceInteractableData.Name = FText::FromString(Info.ItemName);
	//InstanceInteractableData.Action = FText::FromString(TEXT("획득"));
	///*InstanceInteractableData.Quantity = ItemReference->Quantity;*/
	//UpdateInteractableData();
	if (!ItemDrop)
	{
		return;
	}

	ItemReference = ItemDrop;
	ItemReference->SetQuantity(1);

	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

	PickupMesh->SetStaticMesh(Info.Mesh);
	UpdateInteractableData();

	// 필요하면 메쉬도 세팅
	if (PickupMesh && Info.Mesh)
	{
		PickupMesh->SetStaticMesh(Info.Mesh);
	}
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
	InitializePickup(UVMEquipment::StaticClass());
}

void AVMPickup::BeingInteract(AVMCharacterHeroBase* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("AVMPickup::BeingInteract called"));
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
	if (!IsPendingKillPending())
	{
		if (ItemReference)
		{
			if (TObjectPtr<UVMInventoryComponent> PlayerInventory = Taker->GetInventory())
			{
				const FItemAddResult AddResult = PlayerInventory->HandleAddItem(ItemReference);

				if (AddResult.OperationResult != EItemAddResult::IAR_NoItemAdded)
				{
					if (APlayerController* PC = Cast<APlayerController>(Taker->GetController()))
					{
						if (AVMCharacterHeroHUD* HUD = PC->GetHUD<AVMCharacterHeroHUD>())
						{
							if (UVMInventoryPanel* Panel = HUD->GetInventoryPanel())
							{
								Panel->RefreshInventory();
							}
						}
					}
				}

				switch (AddResult.OperationResult)
				{
				case EItemAddResult::IAR_NoItemAdded:
					break;
				case EItemAddResult::IAR_PartialAmountItemAdded:
					UpdateInteractableData();
					Taker->UpdateInteractionWidget();
					break;
				case EItemAddResult::IAR_AllItemAdded:
					Destroy();
					break;
				}

				UE_LOG(LogTemp, Warning, TEXT("%s"), *AddResult.ResultMessage.ToString());
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Player inventory component is null"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Pickup internal item reference was somehow null!"));
		}
	}
}

void AVMPickup::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName ChangedPropertyName =
		PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None;

	if (ChangedPropertyName == GET_MEMBER_NAME_CHECKED(AVMPickup, DesiredItemID))
	{
		if (ItemDataTable)
		{
			const FString ContextString{ DesiredItemID.ToString() };

			if (const FVMEquipmentInfo* ItemData = ItemDataTable->FindRow<FVMEquipmentInfo>(DesiredItemID, DesiredItemID.ToString()))
			{
				PickupMesh->SetStaticMesh(ItemData->Mesh);
			}
		}
	}
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