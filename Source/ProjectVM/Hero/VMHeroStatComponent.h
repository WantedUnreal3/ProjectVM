// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Hero/HeroStat.h"
#include "Components/ActorComponent.h"
#include "VMHeroStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHPChangedDeleagte, float /* CurrentHP */);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTVM_API UVMHeroStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVMHeroStatComponent();

	FOnDeathDelegate OnDeath;
	FOnHPChangedDeleagte OnHPChanged;

	float ApplyDamage(float InDamage);

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat DefaultStat;

	UPROPERTY(VisibleAnywhere, Category = Stat)
	FHeroStat CurStat;
};
