// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VMDragItemVisual.generated.h"

class UVMEquipment;
class UBorder;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTVM_API UVMDragItemVisual : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	//UBorder* ItemBorder;
	TObjectPtr<UBorder> ItemBorder;

	UPROPERTY(VisibleAnywhere, Category = "Drag Item Visual", meta = (BindWidget))
	//UImage* ItemIcon;
	TObjectPtr<UImage> ItemIcon;
};
