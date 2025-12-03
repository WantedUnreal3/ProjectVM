// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/VMInventoryComponent.h"
#include "Item/Equipment/VMEquipment.h"
#include "Inventory/VMPickup.h"
#include "Item/Equipment/VMEquipmentInfo.h"

bool UVMInventoryComponent::CombineAndAddToInventory(UVMEquipment* ItemA, UVMEquipment* ItemB, UVMEquipment*& OutResultItem)
{
	return false;
}

// Sets default values for this component's properties
UVMInventoryComponent::UVMInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UDataTable> DT_Equip(
		TEXT("/ Game / Project / Item / VMEquipmentInfo.VMEquipmentInfo")); // 실제 경로로 수정

	if (DT_Equip.Succeeded())
	{
		EquipmentDataTable = DT_Equip.Object;
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: EquipmentDataTable SET in ctor: %s"),
			*EquipmentDataTable->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Failed to load EquipmentDataTable"));
	}

	//static ConstructorHelpers::FObjectFinder<UDataTable> DT_Equip(
	//	TEXT("/Game/Data/DT_VMEquipmentInfo.DT_VMEquipmentInfo")); // 실제 경로로 수정

	//if (DT_Equip.Succeeded())
	//{
	//	EquipmentDataTable = DT_Equip.Object;
	//	UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: EquipmentDataTable SET in ctor: %s"),
	//		*EquipmentDataTable->GetName());
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("InventoryComponent: Failed to load EquipmentDataTable"));
	//}

	//// ...
}

// Called when the game starts
void UVMInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("InvComp BeginPlay: this=%p, DT=%p"),
	//	this, EquipmentDataTable);

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
		if (const TArray<UVMEquipment*>::ElementType* Result = InventoryContents.FindByKey(ItemIn))
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

void UVMInventoryComponent::DropItemToWorld(UVMEquipment* ItemToDrop, int32 Quantity)
{
	if (!ItemToDrop || !GetOwner() || !GetWorld())
		return;

	// 1) 드랍 위치 계산: 플레이어 앞 150cm 지점
	const FVector OwnerLocation = GetOwner()->GetActorLocation();
	const FVector Forward = GetOwner()->GetActorForwardVector();
	const FVector DropLocation = OwnerLocation + Forward * 150.0f;
	const FRotator DropRotation = FRotator::ZeroRotator;

	// 2) Pickup 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();

	TSubclassOf<AVMPickup> ClassToSpawn = DefaultPickupClass;
	if (!ClassToSpawn)
	{
		ClassToSpawn = AVMPickup::StaticClass();
	}

	AVMPickup* Pickup = GetWorld()->SpawnActor<AVMPickup>(
		AVMPickup::StaticClass(),   // 필요하면 PickupClass UPROPERTY 만들어서 사용
		DropLocation,
		DropRotation,
		SpawnParams
	);

	if (Pickup)
	{
		// 🔹 여기서는 APickup이 이미 DataTable/RowName을 들고 있다고 가정하고
		//    그냥 UVMEquipment 타입과 수량만 넘겨준다.
		Pickup->InitializePickup(ItemToDrop->GetClass());
		
		UE_LOG(LogTemp, Warning,
			TEXT("DropItemToWorld: Spawned pickup for '%s' x%d"),
			*ItemToDrop->GetName(), Quantity);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("DropItemToWorld: SpawnActor<APickup> failed"));
	}

	// 3) 인벤토리에서 제거 (네가 이미 갖고 있는 함수 이름에 맞춰 사용)
	RemoveSingleInstanceOfItem(ItemToDrop);
}

UVMEquipment* UVMInventoryComponent::CombineItems(UVMEquipment* ItemA, UVMEquipment* ItemB)
{
	UE_LOG(LogTemp, Warning, TEXT("CombineItems CALLED"));
	UE_LOG(LogTemp, Warning,
		TEXT("CombineItems: A=%p, B=%p"), ItemA, ItemB);

	// 1) 널 체크
	if (!ItemA || !ItemB)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombineItems FAILED: Null item detected."));
		return nullptr;
	}

	// 2) ID 추출
	const int32 ID_A = ItemA->EquipmentInfo.ItemID;
	const int32 ID_B = ItemB->EquipmentInfo.ItemID;

	// 3) 허용 ID 체크
	const TArray<int32> AllowedIDs = { 1, 2, 4, 8, 16, 32 };
	if (!AllowedIDs.Contains(ID_A) || !AllowedIDs.Contains(ID_B))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("CombineItems FAILED: ItemID not allowed. A=%d, B=%d"), ID_A, ID_B);
		return nullptr;
	}

	// 4) NewID 계산
	int32 NewID = 0;
	if (ID_A == ID_B)
	{
		// 동일한 ID → *1000 조합식
		NewID = ID_A * 1000;
	}
	else
	{
		// 서로 다른 ID → 둘의 합
		NewID = ID_A + ID_B;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("CombineItems SUCCESS: (%d, %d) → NewID = %d"),
		ID_A, ID_B, NewID);

	// 5) DataTable에서 결과 아이템 정보 찾기
	if (!EquipmentDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("CombineItems FAILED: EquipmentDataTable is NULL"));
		return nullptr;
	}

	FVMEquipmentInfo* NewRow =
		EquipmentDataTable->FindRow<FVMEquipmentInfo>(FName(*FString::FromInt(NewID)), TEXT("Combine"));



	if (!NewRow)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("CombineItems FAILED: No item with NewID %d"),
			NewID);
		return nullptr;
	}

	UE_LOG(LogTemp, Warning,
		TEXT("CombineItems: A=%s, B=%s"),
		*GetNameSafe(ItemA),
		*GetNameSafe(ItemB));

	// 6) 결과 아이템 생성
	UVMEquipment* NewItem = NewObject<UVMEquipment>(this, UVMEquipment::StaticClass());
	NewItem->EquipmentInfo = *NewRow;

	// 7) 인벤토리에서 재료 제거
	// 여기 함수 이름은 프로젝트에 맞춰서 바꿔줘.
	// 예: RemoveEquipmentFromInventory / RemoveItem 등
	RemoveEquipmentFromInventory(ItemA);
	RemoveEquipmentFromInventory(ItemB);

	// 8) 결과 아이템 인벤토리에 추가
	AddEquipmentToInventory(NewItem);

	UE_LOG(LogTemp, Warning,
		TEXT("CombineItems: Result item (ID=%d) added to inventory."), NewID);

	return NewItem;
}

void UVMInventoryComponent::AddEquipmentToInventory(UVMEquipment* NewItem)
{
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

void UVMInventoryComponent::UpdateUI()
{
	OnInventoryUpdated.Broadcast();
}
void UVMInventoryComponent::RemoveEquipmentFromInventory(UVMEquipment* Item)
{
	if (!Item)
	{
		return;
	}

	// 배열 이름은 네가 실제로 쓰는 이름으로 바꿔줘.
	// 예: EquipmentList, InventoryEquipments, EquipmentContents 등
	int32 RemovedCount = EquipmentList.Remove(Item);

	UE_LOG(LogTemp, Warning,
		TEXT("RemoveEquipmentFromInventory: Item=%s, Removed=%d"),
		*GetNameSafe(Item),
		RemovedCount);
}

