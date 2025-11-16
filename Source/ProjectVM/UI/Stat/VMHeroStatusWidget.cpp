// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stat/VMHeroStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Hero/VMHeroStatComponent.h"

UVMHeroStatusWidget::UVMHeroStatusWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UVMHeroStatusWidget::RefreshHealthPointBar(float Percentage)
{
	if (HealthPointBar == nullptr)
	{
		return;
	}

	HealthPointBar->SetPercent(Percentage);
}

void UVMHeroStatusWidget::RefreshManaPointBar(float Percentage)
{
	if (ManaPointBar == nullptr)
	{
		return;
	}

	ManaPointBar->SetPercent(Percentage);
}

void UVMHeroStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HealthPointBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("HealthPoint")));
	ManaPointBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ManaPoint")));

	RefreshHealthPointBar(1.0f);
	RefreshManaPointBar(1.0f);
}
