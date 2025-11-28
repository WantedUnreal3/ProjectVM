// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "UObject/NoExportTypes.h"
#include "Hero/VMCharacterHeroBase.h"
#include "VMEquipment.generated.h"


class UVMInventoryComponent;
class UVMEquipmentInfo;
/**
 * 
 */

UCLASS()
class PROJECTVM_API UVMEquipment : public UObject
{
	GENERATED_BODY()

public:

	UVMEquipment();

	void ResetItemFlags();

	TObjectPtr<UVMEquipment> CreateItemCopy() const;

	//FORCEINLINE float GetItemStackWeight() const { return Quantity * ItemNumericData.Weight; };
	//FORCEINLINE float GetItemSingleWeight() const { return ItemNumericData.Weight; };
	//FORCEINLINE bool IsFullItemStack() const { return Quantity == ItemNumericData.Weight; };

	void SetQuantity(const int32 NewQuantity);

	virtual void Use(AVMCharacterHeroBase* Character);


	void EquipEffect(class AVMCharacterHeroBase* Owner);
	FORCEINLINE FVMEquipmentInfo& GetEquipmentInfo() { return EquipmentInfo; };
	FORCEINLINE void SetEquipmentInfo(FVMEquipmentInfo InEquipmentInfo) { EquipmentInfo = InEquipmentInfo; }

public:
	UPROPERTY()
	TObjectPtr<UVMInventoryComponent> OwningInventory;
	
	UPROPERTY(EditAnywhere)
	bool bIsStackable;
	
	bool bIsCopy;
	bool bIsPickup;

protected:
	friend class UItemFactorySubsystem;
	
	bool operator==(const FName& OtherItemName) const
	{
		return FName(*EquipmentInfo.ItemName) == OtherItemName;
	}

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FVMEquipmentInfo EquipmentInfo;
};
