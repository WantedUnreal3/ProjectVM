// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "VMItemDragDropOperation.generated.h"

class UVMEquipment;
class UVMInventoryComponent;
/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMItemDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UVMEquipment* SourceItem;

	UPROPERTY()
	UVMInventoryComponent* SourceInventory;

	
};
