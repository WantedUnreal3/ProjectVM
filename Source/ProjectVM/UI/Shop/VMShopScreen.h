// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Shop/VMShopComponent.h"
#include "Components/TextBlock.h"
#include "Shop/VMEntryParentInterface.h"
#include "VMShopScreen.generated.h"

/**
 *
 */
UCLASS()
class PROJECTVM_API UVMShopScreen : public UUserWidget, public IVMEntryParentInterface
{
	GENERATED_BODY()
public:
	UVMShopScreen(const FObjectInitializer& ObjectInitializer);
	//Initialize는 WBP 위젯과 연동 전에 호출될 수 있다. 안전하게 NativeConstruct()에서 초기화 진행
	virtual void NativeConstruct() override;

public:
	void SetBuyMode();
	void SetSellMode();


	UFUNCTION()
	void OnGridItemButtonClicked(UVMShopItemWidget* ChildGridWidget);


	void SetShopData(int32 InCurrentMoney, int32 InInventoryCurrentCapacity, int32 InInventoryMaxCapacity, UVMShopComponent* InShopComponent);

	FORCEINLINE void UpdateAllPriceText(int32 Value)
	{
		AllItemPrice = Value;
		if (AllPriceText != nullptr)
		{
			AllPriceText->SetText(FText::AsNumber(AllItemPrice));
		}
	}

	void RemoveListItem(int32 Price, UVMShopItemDataObject* ListObject) override;

private:
	UFUNCTION()
	void OnBuyClicked();

	UFUNCTION()
	void OnSellClicked();

	UFUNCTION()
	void OnExitClicked();

	UFUNCTION()
	void OnTradeClicked();

	//버튼 스타일 변경 함수
	void UpdateButtonStyle();

	//그리드패널 갱신
	void RepackShopGrid();

public:
	// 유니폼 그리드 패널 참조
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UUniformGridPanel> ShopGridPanel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UListView> ShopListView;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> BuyButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> SellButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> CurrentMoneyText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> InventoryCapacityText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> AllPriceText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TradeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> TradeButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButton> ExitButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Button")
	bool bIsBuy = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 GridRowNum = 5;


protected:
	TSubclassOf<class UVMShopItemWidget> ShopItemWidgetClass;

	const TArray<FVMEquipmentInfo>* ShopItems;

	const TArray<UVMEquipment*>* InventoryItems;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 CurrentMoney = 0; //소지금

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 InventoryCurrentCapacity = 0; //현재 인벤토리 용량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 InventoryMaxCapacity = 0; //최대 인벤토리 용량

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 AllItemCount = 0; //현재 리스트뷰 아이템 수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	int32 AllItemPrice = 0; //리스트뷰의 아이템 총 가격
	TObjectPtr<class UVMShopComponent> ShopComponent;
};
