// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Hero/VMCharacterHeroBase.h"
#include "VMEquipmentPanel.generated.h"


class UWrapBox;
class UVMEquipment;
class UVMEquipmentItemSlot;
class UVMInventoryItemSlot;
class UVMInventoryPanel;


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
    //UPROPERTY(meta = (BindWidget))
    //UVMInventoryItemSlot* WeaponSlot;
    virtual void NativeConstruct() override;

public:
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PanelRefs")
	//TObjectPtr<UVMInventoryPanel> LinkedInventoryPanel;

public:
	virtual void NativeOnInitialized() override;

	/** 인벤토리에서 더블 클릭한 장비를 첫 빈 슬롯에 장착 */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	int32 TryEquipToEmptySlot(UVMEquipment* Item);

	/** 모든 슬롯 비우기 (UI만) */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void ClearAllSlots();

	UFUNCTION()
	void HandleEquipmentSlotDoubleClicked(UVMEquipment* Item);

protected:
	/** 슬롯 위젯들이 한 번도 안 만들어졌으면 생성해 주는 함수 */
	void EnsureSlotsInitialized();

protected:
	/** 장비 슬롯들을 넣어 줄 WrapBox (WBP에서 BindWidget) */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWrapBox> SlotWrapBox;

	/** 슬롯으로 사용할 위젯 BP 클래스 (예: WBP_VMInventoryItemSlot) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	TSubclassOf<UVMInventoryItemSlot> ItemBoxClass;

	/** 실제로 생성된 슬롯 위젯 배열 */
	UPROPERTY()
	TArray<TObjectPtr<UVMInventoryItemSlot>> WeaponSlots;

	/** 장비 슬롯 개수 (기본 6칸) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
	int32 NumEquipmentSlots = 6;

};
