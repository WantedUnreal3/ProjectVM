// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item/Equipment/VMEquipment.h"
#include "Item/Equipment/VMEquipmentInfo.h"

#include "VMPickup.generated.h"

class UVMEquipment;
class UDataTable;

UCLASS()
class PROJECTVM_API AVMPickup : public AActor, public IVMInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVMPickup();

	void InitializePickup(const TSubclassOf<UVMEquipment> PickupClass);
	void InitializeDrop(UVMEquipment* ItemDrop);

	virtual void BeginFocus() override;
	virtual void EndFocus() override;

	FORCEINLINE const FVMEquipmentInfo& GetEquipmentInfo() const
	{
		return EquipmentInfo;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	virtual void BeingInteract(AVMCharacterHeroBase* Character) override;

	void UpdateInteractableData();

	UFUNCTION()
	void TakePickup(const AVMCharacterHeroBase* Taker);


protected:
	UPROPERTY(VisibleAnywhere, Category = "Pickup | Components")
	TObjectPtr<UStaticMeshComponent> PickupMesh;


	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item DataBase")
	//UDataTable* ItemDataTable;
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	FName DesiredItemID;

	UPROPERTY(VisibleAnywhere, Category = "Pickup | Item Reference")
	//UItemBase* ItemReference;
	TObjectPtr<UVMEquipment> ItemReference;

	UPROPERTY(EditInstanceOnly, Category = "Pickup | Item Initialization")
	int32 ItemQuantity;

	UPROPERTY(VisibleInstanceOnly, Category = "Pickup | Interaction")
	FInteractableData InstanceInteractableData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item)
	FVMEquipmentInfo EquipmentInfo;
};
