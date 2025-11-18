// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Inventory/VMInventoryItemSlot.h"
#include "VMEquipmentPanel.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMEquipmentPanel : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void RefreshEquipment(AVMCharacterHeroBase* Hero);

protected:
    // 무기칸 슬롯 (재사용 가능)
    UPROPERTY(meta = (BindWidget))
    UVMInventoryItemSlot* WeaponSlot;
};
