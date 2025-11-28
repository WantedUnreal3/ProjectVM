
#include "Shop/VMShopComponent.h"
#include "Game/VMRPGPlayerController.h"
#include "UI/Shop/VMShopScreen.h"
#include "Core/VMLoadManager.h"
#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/ItemFactorySubsystem.h"
#include "Inventory/VMInventoryComponent.h"

UVMShopComponent::UVMShopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UVMShopComponent::SetShop()
{
	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		return;
	}

	UVMShopScreen* ShopScreen = Cast<UVMShopScreen>(PC->GetScreen(EScreenUIType::ShopScreen));
	if (ShopScreen == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("ShopScreen is not found"));
		return;
	}

	//InventoryComp가 nullptr이면 받아옴
	if (InventoryComp == nullptr)
	{
		AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(PC->GetPawn());
		if (Player == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("Player is nullptr"));
			return;
		}

		InventoryComp = Cast<UVMInventoryComponent>(Player->GetInventory());
		if (InventoryComp == nullptr)
		{
			UE_LOG(LogTemp, Log, TEXT("InventoryComponent is nullptr"));
			return;
		}

	}

	InventoryCurrentCapacity = InventoryComp->GetInventoryContents().Num();
	PlayerMoney = InventoryComp->GetMoney();
	ShopScreen->SetShopData(PlayerMoney, InventoryCurrentCapacity, InventoryMaxCapacity, this);
	ShopScreen->SetBuyMode();
}

void UVMShopComponent::SetTestInventory()
{
	//Todo: 인벤토리 기능 구현 되기 전 테스트를 위한 더미데이터 세팅 함수

	//for (int i = 0; i < TestInventoryCurrentCapacity; ++i)
	//{
	//	UVMEquipment* Equipment = GetOwner()->GetGameInstance()->GetSubsystem<UItemFactorySubsystem>()->CreateRandomBaseEquipment();
	//	InventoryItems.Add(Equipment);
	//}
}

void UVMShopComponent::AddInventoryItem(UVMEquipment* NewItem)
{
	if (NewItem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AddInventoryItem: NewItem is nullptr"));
		return;
	}

	InventoryComp->GetInventoryContents().Add(NewItem);
	//InventoryItems.Add(NewItem);
	UE_LOG(LogTemp, Log, TEXT("Item added to InventoryItems: %s"), *NewItem->GetName());
}

void UVMShopComponent::BeginPlay()
{
	Super::BeginPlay();

	//아이템 추가
	for (int i = 1; i < 28; ++i)
	{
		AddShopItem(i);
	}

	//SetTestInventory();
}

void UVMShopComponent::AddShopItem(int32 ItemNum)
{
	FName RowName = FName(*FString::FromInt(ItemNum));
	FVMEquipmentInfo* LoadedData = GetOwner()->GetGameInstance()->GetSubsystem<UVMLoadManager>()->GetEquipmentInfoRow(RowName);
	if (LoadedData != nullptr)
	{
		ShopItems.Add(*LoadedData); // 포인터 → 값 복사
	}
}






