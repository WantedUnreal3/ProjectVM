// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Stat/VMHeroStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Hero/VMHeroStatComponent.h"

UVMHeroStatusWidget::UVMHeroStatusWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UVMHeroStatusWidget::UpdateMaxHealthPoint(int InMaxHealthPoint)
{
	MaxHealthPoint = InMaxHealthPoint;
	HealthPointText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentHealthPoint, MaxHealthPoint)));
	RefreshHealthPointBar(static_cast<float>(CurrentHealthPoint) / MaxHealthPoint);
}

void UVMHeroStatusWidget::UpdateCurrentHealthPoint(int InCurrentHealthPoint)
{
	CurrentHealthPoint = InCurrentHealthPoint;
	HealthPointText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentHealthPoint, MaxHealthPoint)));
	RefreshHealthPointBar(static_cast<float>(CurrentHealthPoint) / MaxHealthPoint);
}

void UVMHeroStatusWidget::UpdateMaxManaPoint(int InMaxManaPoint)
{
	MaxManaPoint = InMaxManaPoint;
	ManaPointText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentManaPoint, MaxManaPoint)));
	RefreshManaPointBar(static_cast<float>(CurrentManaPoint) / MaxManaPoint);
}

void UVMHeroStatusWidget::UpdateCurrentManaPoint(int InCurrentManaPoint)
{
	CurrentManaPoint = InCurrentManaPoint;
	ManaPointText->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), CurrentManaPoint, MaxManaPoint)));
	RefreshManaPointBar(static_cast<float>(CurrentManaPoint) / MaxManaPoint);
}

void UVMHeroStatusWidget::RefreshHealthPointBar(float Percentage)
{
	if (HealthPoint == nullptr)
	{
		return;
	}

	HealthPoint->SetPercent(Percentage);
}

void UVMHeroStatusWidget::RefreshManaPointBar(float Percentage)
{
	if (ManaPoint == nullptr)
	{
		return;
	}

	ManaPoint->SetPercent(Percentage);
}

void UVMHeroStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RefreshHealthPointBar(1.0f);
	RefreshManaPointBar(1.0f);
}
