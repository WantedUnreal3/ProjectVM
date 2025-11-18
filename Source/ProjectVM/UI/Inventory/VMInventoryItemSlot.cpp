// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMInventoryItemSlot.h"
#include "UI/Inventory/VMInventoryTooltip.h"
#include "UI/Inventory/VMDragItemVisual.h"
#include "Inventory/VMItemDragDropOperation.h"

#include "Item/Equipment/VMEquipmentInfo.h"
#include "Item/Equipment/VMEquipment.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"




void UVMInventoryItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (TooltipClass)
	{
		UVMInventoryTooltip* ToolTip = CreateWidget<UVMInventoryTooltip>(this, TooltipClass);
		ToolTip->InventorySlotBeingHovered = this;
		SetToolTip(ToolTip);
	}
}

void UVMInventoryItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();
	/*if (ItemReference)
	{
		ItemBorder->SetBrushColor(FLinearColor::Gray);
	}

	ItemIcon->SetBrushFromTexture(Info.Icon);

	if (!ItemReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryItemSlot: ItemReference is null in NativeConstruct"));
		return;
	}


	ItemBorder->SetBrushColor(FLinearColor::Gray);

	if (Info.Icon)
	{
		ItemIcon->SetBrushFromTexture(Info.Icon);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("InventoryItemSlot: Icon is null for item %s"),
			*Info.ItemName);
	}
	*/

	if (ItemBorder)
	{
		ItemBorder->SetBrushColor(FLinearColor::Transparent);
	}

}

FReply UVMInventoryItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("Slot::OnMouseButtonDown"));

	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		return Reply.Handled().DetectDrag(TakeWidget(), EKeys::LeftMouseButton);
	}

	return Reply.Unhandled();
}

void UVMInventoryItemSlot::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
}

void UVMInventoryItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	/*Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

	if (DragItemVisualClass)
	{
		const TObjectPtr<UVMDragItemVisual> DragVisual = CreateWidget<UVMDragItemVisual>(this, DragItemVisualClass);
		DragVisual->ItemIcon->SetBrushFromTexture(Info.Icon);
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		TObjectPtr<UVMItemDragDropOperation> DragItemOperation = NewObject<UVMItemDragDropOperation>();
		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;

		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;
	}*/
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);

	if (!ItemReference)
		return;

	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

	if (DragItemVisualClass)
	{
		UVMDragItemVisual* DragVisual =
			CreateWidget<UVMDragItemVisual>(this, DragItemVisualClass);

		if (Info.Icon)
		{
			DragVisual->ItemIcon->SetBrushFromTexture(Info.Icon);
		}
		DragVisual->ItemBorder->SetBrushColor(ItemBorder->GetBrushColor());

		UVMItemDragDropOperation* DragItemOperation =
			NewObject<UVMItemDragDropOperation>();

		DragItemOperation->SourceItem = ItemReference;
		DragItemOperation->SourceInventory = ItemReference->OwningInventory;
		DragItemOperation->DefaultDragVisual = DragVisual;
		DragItemOperation->Pivot = EDragPivot::TopLeft;

		OutOperation = DragItemOperation;

		UE_LOG(LogTemp, Warning, TEXT("NativeOnDragDetected: drag op created for %s"),
			*Info.ItemName);

	}
}

void UVMInventoryItemSlot::SetItemReference(UVMEquipment* ItemIn)
	{ 
		ItemReference = ItemIn;
		const TCHAR* NameForLog = TEXT("NULL");
		if (ItemReference)
		{
			// ItemName 이 FString 이라면 이렇게 TCHAR* 로 꺼내야 함
			NameForLog = *ItemReference->GetEquipmentInfo().ItemName;
		}

		UE_LOG(LogTemp, Warning, TEXT("SetItemReference called, Item: %s"), NameForLog);

		RefreshFromItem();

		SetUp(ItemIn->GetEquipmentInfo());
	};


void UVMInventoryItemSlot::SetUp(const FVMEquipmentInfo& Info)
{
	// 다이나믹 머터리얼 생성
	if (ItemIcon && ItemIcon->GetBrush().GetResourceObject())
	{
		UMaterialInterface* BaseMat = Cast<UMaterialInterface>(ItemIcon->GetBrush().GetResourceObject());
		if (BaseMat != nullptr)
		{
			ItemMaterialInstance = UMaterialInstanceDynamic::Create(BaseMat, this);
			ItemIcon->SetBrushFromMaterial(ItemMaterialInstance);
		}
	}
	//머터리얼 파라미터 설정
	if (ItemMaterialInstance)
	{
		ItemMaterialInstance->SetScalarParameterValue(TEXT("ColumnIndex"), Info.AtlasCol);
		ItemMaterialInstance->SetScalarParameterValue(TEXT("RowIndex"), Info.AtlasRow);
	}

}

void UVMInventoryItemSlot::RefreshFromItem()
{
	UE_LOG(LogTemp, Warning, TEXT("RefreshFromItem: ENTER"));

	// 1) 위젯 바인딩이 제대로 되었는지 확인
	if (!ItemBorder)
	{
		return;
	}

	// 2) 슬롯에 아이템이 없는 경우 → 빈 슬롯으로 표시
	if (!ItemReference)
	{
		UE_LOG(LogTemp, Warning, TEXT("RefreshFromItem: ItemReference is NULL"));
		ItemBorder->SetBrushColor(FLinearColor::Transparent);
		//ItemIcon->SetBrushFromTexture(nullptr);
		return;
	}

	// 3) 실제 아이템 정보 가져오기
	const FVMEquipmentInfo& Info = ItemReference->GetEquipmentInfo();

	// 테두리 색 (일단 회색으로 고정, 나중에 등급별 색상 적용 가능)
	ItemBorder->SetBrushColor(FLinearColor::Gray);

	// 4) 아이콘 세팅
	/*if (Info.Icon)
	{
		ItemIcon->SetBrushFromTexture(Info.Icon);
		UE_LOG(LogTemp, Warning, TEXT("RefreshFromItem: Icon %s set for %s"),
			*Info.Icon->GetName(), *Info.ItemName);
	}
	else
	{
		ItemIcon->SetBrushFromTexture(nullptr);
		UE_LOG(LogTemp, Warning, TEXT("RefreshFromItem: Icon is NULL for %s"),
			*Info.ItemName);
	}*/

}




bool UVMInventoryItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

FReply UVMInventoryItemSlot::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("InventorySlot::OnMouseButtonDoubleClick"));

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton && ItemReference)
	{
		// 더블클릭 시 장착 요청 브로드캐스트
		OnItemDoubleClicked.Broadcast(ItemReference);
		return FReply::Handled();
	}

	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}

