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

	void SetQuantity(const int32 NewQuantity);

	virtual void Use(AVMCharacterHeroBase* Character);


	void EquipEffect(class AVMCharacterHeroBase* Owner);
	FORCEINLINE const FVMEquipmentInfo& GetEquipmentInfo() const { return EquipmentInfo; };
	FORCEINLINE void SetEquipmentInfo(FVMEquipmentInfo InEquipmentInfo) 
	{ 
		EquipmentInfo = InEquipmentInfo; 
		UE_LOG(LogTemp, Warning, TEXT("UVMEquipment::SetEquipmentInfo: %s Icon:%s"),
			*EquipmentInfo.ItemName,
			EquipmentInfo.Icon ? *EquipmentInfo.Icon->GetName() : TEXT("NULL"));
	}

public:
	UPROPERTY()
	TObjectPtr<UVMInventoryComponent> OwningInventory;
	
	UPROPERTY(EditAnywhere)
	bool bIsStackable;
	
	bool bIsCopy;
	bool bIsPickup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FVMEquipmentInfo EquipmentInfo;

protected:
	friend class UItemFactorySubsystem;
	
	bool operator==(const FName& OtherItemName) const
	{
		return FName(*EquipmentInfo.ItemName) == OtherItemName;
	}

protected:
	
};
