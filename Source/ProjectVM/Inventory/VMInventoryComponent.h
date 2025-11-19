// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VMInventoryComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnInventoryUpdated);


class UVMEquipment;

UENUM(BlueprintType)
enum class EItemAddResult : uint8
{
	IAR_NoItemAdded UMETA(DisplayName = "No item added"),
	IAR_PartialAmountItemAdded UMETA(DisplayName = "Partial amount of item added"),
	IAR_AllItemAdded UMETA(DisplayName = "All of item added")
};

USTRUCT(BlueprintType)
struct FItemAddResult
{
	GENERATED_BODY()

	FItemAddResult() :
		ActualAmountAdded(0),
		OperationResult(EItemAddResult::IAR_NoItemAdded),
		ResultMessage(FText::GetEmpty())
	{
	};

	// Actual amount of item that was added to the inventory
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	int32 ActualAmountAdded;

	// Enum representing the end state of an add item operation
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	EItemAddResult OperationResult;

	// Informational message that can be passed with the result 
	UPROPERTY(BlueprintReadOnly, Category = "Item Add Result")
	FText ResultMessage;

	static FItemAddResult AddedNone(const FText& ErrorText)
	{
		FItemAddResult AddedNoneResult;
		AddedNoneResult.ActualAmountAdded = 0;
		AddedNoneResult.OperationResult = EItemAddResult::IAR_NoItemAdded;
		AddedNoneResult.ResultMessage = ErrorText;
		return AddedNoneResult;
	};
	static FItemAddResult AddedPartial(const int32 PartialAomountedAdded, const FText& ErrorText)
	{
		FItemAddResult AddedPartialResult;
		AddedPartialResult.ActualAmountAdded = PartialAomountedAdded;
		AddedPartialResult.OperationResult = EItemAddResult::IAR_PartialAmountItemAdded;
		AddedPartialResult.ResultMessage = ErrorText;
		return AddedPartialResult;
	};
	static FItemAddResult AddedAll(const int32 AmountAdded, const FText& Message)
	{
		FItemAddResult AddedAllResult;
		AddedAllResult.ActualAmountAdded = AmountAdded;
		AddedAllResult.OperationResult = EItemAddResult::IAR_AllItemAdded;
		AddedAllResult.ResultMessage = Message;
		return AddedAllResult;
	};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVM_API UVMInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	FOnInventoryUpdated OnInventoryUpdated;


public:
	// Sets default values for this component's properties
	UVMInventoryComponent();

	UFUNCTION(Category = "Inventory")
	UVMEquipment* FindMatchingItem(UVMEquipment* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UVMEquipment* FindNextItemByID(UVMEquipment* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	UVMEquipment* FindNextPartialStack(UVMEquipment* ItemIn) const;

	UFUNCTION(Category = "Inventory")
	FItemAddResult HandleAddItem(UVMEquipment* InputItem);

	UFUNCTION(Category = "Inventory")
	void RemoveSingleInstanceOfItem(UVMEquipment* ItemToRemove);

	UFUNCTION(Category = "Inventory")
	int32 RemoveAmountOfItem(UVMEquipment* ItemIn, int32 DesiredAmountToRemove);

	UFUNCTION(Category = "Inventory")
	void SplitExistingStack(UVMEquipment* ItemIn, const int32 AmountToSplit);

	UFUNCTION(Category = "Inventory")
	FORCEINLINE TArray<UVMEquipment*> GetInventoryContents() const { return InventoryContents; };

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void RemoveItem(UVMEquipment* Item);

	UPROPERTY()
	int32 SlotCapacity;

	//// Setter
	//UFUNCTION(Category = "Inventory")
	//FORCEINLINE void SetSlotsCapacity(const int32 NewSlotsCapacity) { InventorySlotsCapacity = NewSlotsCapacity; };

	//UFUNCTION(Category = "Inventory")
	//FORCEINLINE void SetWeightCapacity(const int32 NewWeightCapacity) { InventoryWeightCapacity = NewWeightCapacity; };

	void AddNewItem(UVMEquipment* Item, const int32 AmountToAdd);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FItemAddResult HandleNonStackableItems(UVMEquipment* InputItem);


protected:	
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TArray<TObjectPtr<UVMEquipment>> InventoryContents;

	


};
