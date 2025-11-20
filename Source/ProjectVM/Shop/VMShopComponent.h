#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "VMShopComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVM_API UVMShopComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVMShopComponent();

	void SetShop();
	void SetTestInventory();

	//getter
	FORCEINLINE const TArray<FVMEquipmentInfo>& GetShopItems() const
	{
		return ShopItems;
	}
	FORCEINLINE const TArray<UVMEquipment*>& GetInventoryItems() const
	{
		return InventoryComp->GetInventoryContents(); //InventoryItems;
	}

	FORCEINLINE void SetTestValues(int32 InMoney, int32 InCurrentCapacity, int32 InMaxCapacity)
	{
		PlayerMoney = InMoney;
		InventoryCurrentCapacity = InCurrentCapacity;
		InventoryMaxCapacity = InMaxCapacity;
	}

	//구매시 인벤토리에 아이템 추가하는 함수
	void AddInventoryItem(UVMEquipment* NewItem);

	//판매시 인벤토리에 아이템을 지우는 함수
	FORCEINLINE void RemoveInventoryItem(int32 index)
	{
		//InventoryItems.RemoveAt(index);
		InventoryComp->GetInventoryContents().RemoveAt(index);
	}

protected:
	virtual void BeginPlay() override;
	
private:
	void AddShopItem(int32 ItemNum);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VALUE")
	FName ShopId = TEXT("temp");
	
private:
	UPROPERTY()
	TArray<struct FVMEquipmentInfo> ShopItems;

	//UPROPERTY()
	//TArray<class UVMEquipment*> InventoryItems;

	//UPROPERTY()
	TObjectPtr<class UVMInventoryComponent> InventoryComp;


	//Todo: 나중에 인벤토리 붙이면 바꿔야함
	int32 PlayerMoney = 0;
	int32 InventoryCurrentCapacity = 0;
	int32 InventoryMaxCapacity = 20;
};
