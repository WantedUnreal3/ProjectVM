// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Inventory/VMMainMenu.h"
#include "Hero/VMCharacterHeroBase.h"
#include "Item/Equipment/VMEquipment.h"

void UVMMainMenu::NativeOnInitialized()
{
    Super::NativeOnInitialized();
}


void UVMMainMenu::NativeConstruct()
{
    Super::NativeConstruct();

    Character = Cast<AVMCharacterHeroBase>(GetOwningPlayerPawn());
}

bool UVMMainMenu::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
    return false;
}
