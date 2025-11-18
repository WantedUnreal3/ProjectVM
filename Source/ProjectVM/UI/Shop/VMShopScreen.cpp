// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Shop/VMShopScreen.h"
#include "Components/Button.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Game/VMRPGPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Hero/VMCharacterHeroBase.h"
#include "UI/Shop/VMShopItemWidget.h"
#include "UI/Shop/VMShopItemDataObject.h"
#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/Equipment/VMEquipment.h"
#include "Shop/VMShopComponent.h"
#include "Item/ItemFactorySubsystem.h"

UVMShopScreen::UVMShopScreen(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UVMShopItemWidget> ShopItemWidgetRef(TEXT("/Game/Project/UI/Shop/WBP_ShopItemWidget.WBP_ShopItemWidget_C"));
	if (ShopItemWidgetRef.Succeeded())
	{
		ShopItemWidgetClass = ShopItemWidgetRef.Class;
	}
}

void UVMShopScreen::NativeConstruct()
{
	//버튼 함수 바인딩
	if (BuyButton != nullptr)
	{
		BuyButton->OnClicked.AddDynamic(this, &UVMShopScreen::OnBuyClicked);
	}

	if (SellButton != nullptr)
	{
		SellButton->OnClicked.AddDynamic(this, &UVMShopScreen::OnSellClicked);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &UVMShopScreen::OnExitClicked);
	}

	if (TradeButton != nullptr)
	{
		TradeButton->OnClicked.AddDynamic(this, &UVMShopScreen::OnTradeClicked);
	}

	UpdateButtonStyle();
}

void UVMShopScreen::SetBuyMode()
{
	UE_LOG(LogTemp, Log, TEXT("Open Buy Shop!!"));

	//ShopItems = &NewShopItems;
	if (ShopComponent == nullptr)
	{
		return;
	}
	ShopItems = &ShopComponent->GetShopItems();

	if (ShopGridPanel == nullptr)
	{
		return;
	}
	if (ShopItemWidgetClass == nullptr)
	{
		return;
	}

	ShopGridPanel->ClearChildren(); //그리드 패널 재설정

	int32 Index = 0;
	for (const FVMEquipmentInfo& Item : *ShopItems)
	{
		UE_LOG(LogTemp, Log, TEXT("Item : %s"), *Item.ItemName);
		UVMShopItemWidget* NewItemWidget = CreateWidget<UVMShopItemWidget>(this, ShopItemWidgetClass);

		NewItemWidget->SetUp(Item);
		NewItemWidget->ShopScreen = this;
		NewItemWidget->InventoryIndex = Index;

		//그리드 5열
		int32 Row = Index / GridRowNum;
		int32 Col = Index % GridRowNum;
		ShopGridPanel->AddChildToUniformGrid(NewItemWidget, Row, Col);
		Index++;
	}

}

void UVMShopScreen::SetSellMode()
{
	UE_LOG(LogTemp, Log, TEXT("Open Sell Shop!!"));
	if (ShopComponent == nullptr)
	{
		return;
	}

	InventoryItems = &ShopComponent->GetInventoryItems();
	//InventoryItems = &NewInventoryItems;

	if (ShopGridPanel == nullptr)
	{
		return;
	}
	if (ShopItemWidgetClass == nullptr)
	{
		return;
	}

	ShopGridPanel->ClearChildren(); //그리드 패널 재설정

	if (!InventoryItems)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryItems is nullptr!"));
		return; // 또는 적절히 처리
	}

	int32 Index = 0;
	for (UVMEquipment* Item : *InventoryItems)
	{
		UE_LOG(LogTemp, Log, TEXT("Item : %s"), *Item->GetEquipmentInfo().ItemName);
		UVMShopItemWidget* NewItemWidget = CreateWidget<UVMShopItemWidget>(this, ShopItemWidgetClass);

		NewItemWidget->SetUp(Item->GetEquipmentInfo());
		NewItemWidget->ShopScreen = this;
		NewItemWidget->InventoryIndex = Index;
		//그리드 5열
		int32 Row = Index / GridRowNum;
		int32 Col = Index % GridRowNum;
		ShopGridPanel->AddChildToUniformGrid(NewItemWidget, Row, Col);
		Index++;
	}
}

//void UVMShopScreen::OnGridItemButtonClicked(const FVMEquipmentInfo& ClickedItemInfo)
//{
//	UE_LOG(LogTemp, Log, TEXT("Clicked: %s"), *ClickedItemInfo.ItemName);
//	TArray<UObject*> Items = ShopListView->GetListItems();
//	bool bIsFind = false;
//
//	// 아이템이 이미 리스트에 추가되어있는지 확인
//	for (UObject* ItemObject : Items)
//	{
//		UVMShopItemDataObject* ItemData = Cast<UVMShopItemDataObject>(ItemObject);
//		if (ItemData != nullptr)
//		{
//			if (ItemData->EquipmentInfo->ItemID == ClickedItemInfo.ItemID)
//			{
//				bIsFind = true;
//				//추가 되어있다면 새로 만들지말고 기존 아이템 Count 증가
//				if (ItemData->ChangeItemCount(true))
//				{
//					//추가 성공 했으면 Count 증가
//					++AllItemCount;
//				}
//				break;
//			}
//		}
//	}
//
//	//추가되어 있지않다며 새로 아이템 추가
//	if (!bIsFind)
//	{
//		UVMShopItemDataObject* NewShopItemDataObject = NewObject<UVMShopItemDataObject>(this);
//		NewShopItemDataObject->EquipmentInfo = &ClickedItemInfo;
//		NewShopItemDataObject->ParentInterface = this; //인터페이스 타입으로 자기자신을 넘김
//		ShopListView->AddItem(NewShopItemDataObject);
//		++AllItemCount;
//	}
//
//	//만약 판매 화면이면 그리드 패널 클릭시 아이템 삭제
//	if (!bIsBuy)
//	{
//
//	}
//
//
//	UpdateAllPriceText(AllItemPrice + ClickedItemInfo.ItemLevel * 2000);
//}

void UVMShopScreen::OnGridItemButtonClicked(UVMShopItemWidget* ChildGridWidget)
{
	UE_LOG(LogTemp, Log, TEXT("GridItemClicked: %s"), *(ChildGridWidget->GetEquipmentInfo()->ItemName));
	//클릭된 그리드버튼의 인덱스 확인
	int32 GridIndex = -1;
	for (int32 i = 0; i < ShopGridPanel->GetChildrenCount(); i++)
	{
		if (ShopGridPanel->GetChildAt(i) == ChildGridWidget)
		{
			GridIndex = i;
			break;
		}
	}
	if (GridIndex < 0)
	{
		UE_LOG(LogTemp, Log, TEXT("index가 음수가 되는 말도 안되는 일 발생"));
		return;
	}

	TArray<UObject*> Items = ShopListView->GetListItems();
	bool bIsFind = false;

	// 아이템이 이미 리스트에 추가되어있는지 확인
	for (UObject* ItemObject : Items)
	{
		UVMShopItemDataObject* ItemData = Cast<UVMShopItemDataObject>(ItemObject);
		if (ItemData != nullptr)
		{
			if (ItemData->EquipmentInfo->ItemID == ChildGridWidget->GetEquipmentInfo()->ItemID)
			{
				bIsFind = true;
				//추가 되어있다면 새로 만들지말고 기존 아이템 Count 증가
				if (ItemData->ChangeItemCount(true))
				{
					//추가 성공 했으면 Count 증가
					++AllItemCount;
					UpdateAllPriceText(AllItemPrice + ChildGridWidget->GetEquipmentInfo()->ItemLevel * 2000);
				}
				ItemData->InventoryIndexes.Add(ChildGridWidget->InventoryIndex);
				break;
			}
		}
	}

	//추가되어 있지않다며 새로 아이템 추가
	if (!bIsFind)
	{
		UVMShopItemDataObject* NewShopItemDataObject = NewObject<UVMShopItemDataObject>(this);
		NewShopItemDataObject->EquipmentInfo = ChildGridWidget->GetEquipmentInfo();
		NewShopItemDataObject->ParentInterface = this; //인터페이스 타입으로 자기자신을 넘김
		NewShopItemDataObject->InventoryIndexes.Add(ChildGridWidget->InventoryIndex);
		ShopListView->AddItem(NewShopItemDataObject);
		++AllItemCount;
		UpdateAllPriceText(AllItemPrice + ChildGridWidget->GetEquipmentInfo()->ItemLevel * 2000);
	}

	//판매화면이면 클릭시 그리드뷰에서 제거
	if (!bIsBuy)
	{
		ShopGridPanel->RemoveChild(ChildGridWidget);
		RepackShopGrid();
	}
}

void UVMShopScreen::SetShopData(int32 InCurrentMoney, int32 InInventoryCurrentCapacity, int32 InInventoryMaxCapacity, UVMShopComponent* InShopComponent)
{
	CurrentMoney = InCurrentMoney;
	InventoryCurrentCapacity = InInventoryCurrentCapacity;
	InventoryMaxCapacity = InInventoryMaxCapacity;
	ShopComponent = InShopComponent;

	if (CurrentMoneyText != nullptr)
	{
		CurrentMoneyText->SetText(FText::AsNumber(CurrentMoney));
	}

	if (InventoryCapacityText != nullptr)
	{
		InventoryCapacityText->SetText(
			FText::Format(
				FText::FromString(TEXT("{0} / {1}")),
				FText::AsNumber(InventoryCurrentCapacity),
				FText::AsNumber(InventoryMaxCapacity)
			)
		);
	}
	UpdateAllPriceText(0);
}

void UVMShopScreen::RemoveListItem(int32 Price, UVMShopItemDataObject* ListObject)
{
	//아이템 판매 UI 반영
	AllItemPrice += Price;
	if (AllPriceText != nullptr)
	{
		AllPriceText->SetText(FText::AsNumber(AllItemPrice));
	}
	--AllItemCount;

	UE_LOG(LogTemp, Log, TEXT("deldte : %s"), *ListObject->EquipmentInfo->ItemName);
	//그리드 뷰에 아이템 추가
	
	UVMShopItemWidget* NewItemWidget = CreateWidget<UVMShopItemWidget>(this, ShopItemWidgetClass);
	NewItemWidget->SetUp(*(ListObject->EquipmentInfo));

	NewItemWidget->ShopScreen = this;
	int32 NowIndex = ListObject->InventoryIndexes[0];
	ListObject->InventoryIndexes.RemoveAt(0);
	NewItemWidget->InventoryIndex = NowIndex;

	int32 ChildCount = ShopGridPanel->GetChildrenCount();
	int32 Row = ChildCount / GridRowNum;
	int32 Col = ChildCount % GridRowNum;
	ShopGridPanel->AddChildToUniformGrid(NewItemWidget, Row, Col);
}

void UVMShopScreen::OnBuyClicked()
{
	if (bIsBuy)
	{
		return;
	}

	bIsBuy = true;
	UpdateButtonStyle();

	SetBuyMode();
	ShopListView->ClearListItems();
	AllItemCount = 0;
	UpdateAllPriceText(0);
}

void UVMShopScreen::OnSellClicked()
{
	if (!bIsBuy)
	{
		return;
	}

	bIsBuy = false;
	UpdateButtonStyle();

	SetSellMode();
	//ShopGridPanel->ClearChildren();
	ShopListView->ClearListItems();
	AllItemCount = 0;
	UpdateAllPriceText(0);
}

void UVMShopScreen::OnTradeClicked()
{
	//구매 화면이면 가능 여부 판단. 
	if (bIsBuy)
	{
		if (CurrentMoney < AllItemPrice)
		{
			UE_LOG(LogTemp, Log, TEXT("소지금 부족"));
			return;
		}

		if (InventoryMaxCapacity - InventoryCurrentCapacity < AllItemCount)
		{
			UE_LOG(LogTemp, Log, TEXT("인벤토리 공간 부족"));
			return;
		}

		//아이템 구매 UI 반영
		CurrentMoney -= AllItemPrice;
		CurrentMoneyText->SetText(FText::AsNumber(CurrentMoney));
		InventoryCurrentCapacity += AllItemCount;
		InventoryCapacityText->SetText(
			FText::Format(
				FText::FromString(TEXT("{0} / {1}")),
				FText::AsNumber(InventoryCurrentCapacity),
				FText::AsNumber(InventoryMaxCapacity)
			)
		);

		//리스트뷰 순회하며 인벤토리에 아이템 추가
		const TArray<UObject*>& Items = ShopListView->GetListItems();
		for (UObject* ItemObj : Items)
		{
			UVMShopItemDataObject* EquipData = Cast<UVMShopItemDataObject>(ItemObj);
			if (!EquipData || !EquipData->EquipmentInfo)
			{
				continue;
			}
			const FName NewItemName = FName(EquipData->EquipmentInfo->ItemName);

			for (int i = 0; i < EquipData->CurrentCount; ++i)
			{
				UVMEquipment* Equipment = ShopComponent->GetOwner()->GetGameInstance()->GetSubsystem<UItemFactorySubsystem>()->CreateEquipmentByName(NewItemName);
				if (Equipment != nullptr)
				{
					ShopComponent->AddInventoryItem(Equipment);
				}
			}
		}
	}
	else
	{
		//아이템 구매 UI 반영
		CurrentMoney += AllItemPrice;
		CurrentMoneyText->SetText(FText::AsNumber(CurrentMoney));
		InventoryCurrentCapacity -= AllItemCount;
		InventoryCapacityText->SetText(
			FText::Format(
				FText::FromString(TEXT("{0} / {1}")),
				FText::AsNumber(InventoryCurrentCapacity),
				FText::AsNumber(InventoryMaxCapacity)
			)
		);

		TArray<int32> AllIndexes;
		const TArray<UObject*>& Items = ShopListView->GetListItems();
		//리스트 뷰 순회하며 인벤토리 인덱스 Array에 저장
		for (UObject* ItemObj : Items)
		{
			UVMShopItemDataObject* EquipData = Cast<UVMShopItemDataObject>(ItemObj);
			if (!EquipData || !EquipData->EquipmentInfo)
			{
				continue;
			}
			AllIndexes.Append(EquipData->InventoryIndexes);
		}

		AllIndexes.Sort([](int32 A, int32 B) { return A > B; }); //내림차순 정렬

		//아이템 삭제
		for (int32 index : AllIndexes)
		{
			UE_LOG(LogTemp, Warning, TEXT("Removing index: %d / Num: %d"), index, ShopComponent->GetInventoryItems().Num());
			ShopComponent->RemoveInventoryItem(index);
		}

	}

	ShopListView->ClearListItems();
	AllItemCount = 0;
	UpdateAllPriceText(0);
}


void UVMShopScreen::OnExitClicked()
{
	////다음에 들어올 때 구입부터 나오도록 bool 설정
	bIsBuy = true;
	UpdateButtonStyle();

	AVMRPGPlayerController* PC = Cast<AVMRPGPlayerController>(GetWorld()->GetFirstPlayerController());
	if (PC == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerController is not found"));
		return;
	}
	PC->ShowScreen(EScreenUIType::GameScreen);
	PC->bShowMouseCursor = false;
	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter != nullptr)
	{
		AVMCharacterHeroBase* Player = Cast<AVMCharacterHeroBase>(PlayerCharacter);
		if (Player != nullptr)
		{
			Player->ChangeInputMode(EInputMode::Default);
		}
	}

	//ShopComponent nullptr로 초기화
	ShopListView->ClearListItems();
	AllItemCount = 0;
	UpdateAllPriceText(0);
	ShopComponent->SetTestValues(CurrentMoney, InventoryCurrentCapacity, InventoryMaxCapacity);
	ShopComponent = nullptr;

	//시점 다시 돌리기
	PC->SetViewTargetWithBlend(PC->GetPawn(), 0.5f);
}

void UVMShopScreen::UpdateButtonStyle()
{
	FLinearColor SelectColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#072C7FCC"))); //파란색
	FLinearColor UnselectColor = FLinearColor::FromSRGBColor(FColor::FromHex(TEXT("#ADBBB780")));//회색

	if (BuyButton != nullptr)
	{
		BuyButton->SetBackgroundColor(bIsBuy ? SelectColor : UnselectColor);
	}

	if (SellButton != nullptr)
	{
		SellButton->SetBackgroundColor(bIsBuy ? UnselectColor : SelectColor);
	}

	if (TradeText != nullptr)
	{
		TradeText->SetText(FText::FromString(bIsBuy ? TEXT("구입") : TEXT("판매")));
	}
}

void UVMShopScreen::RepackShopGrid()
{
	TArray<UWidget*> Children = ShopGridPanel->GetAllChildren();
	ShopGridPanel->ClearChildren(); //그리드 패널 재설정

	for (int32 i = 0; i < Children.Num(); ++i)
	{
		//index 갱신
		UVMShopItemWidget* ItemWidget = Cast<UVMShopItemWidget>(Children[i]);
		if (ItemWidget)
		{
			ItemWidget->InventoryIndex = i; 
		}
		UUniformGridSlot* GridSlot = Cast<UUniformGridSlot>(ShopGridPanel->AddChild(Children[i]));
		if (GridSlot != nullptr)
		{
			GridSlot->SetRow(i / GridRowNum);
			GridSlot->SetColumn(i % GridRowNum);
		}
	}
}
