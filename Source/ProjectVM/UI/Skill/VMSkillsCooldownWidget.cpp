// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/VMSkillsCooldownWidget.h"

#include "Components/TextBlock.h"
#include "Math/UnitConversion.h"

void UVMSkillsCooldownWidget::StartBasicSkillCooldown(float Time)
{
	BasicSkillCooldown = Time;
	BasicSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), BasicSkillCooldown)));
}

void UVMSkillsCooldownWidget::StartAdvancedSkillCooldown(float Time)
{
	AdvancedSkillCooldown = Time;
	AdvancedSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), AdvancedSkillCooldown)));
}

void UVMSkillsCooldownWidget::StartMovementSkillSkillCooldown(float Time)
{
	MovementSkillCooldown = Time;
	MovementSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), MovementSkillCooldown)));
}

void UVMSkillsCooldownWidget::StartUltimateSkillCooldown(float Time)
{
	UltimateSkillCooldown = Time;
	UltimateSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), UltimateSkillCooldown)));
}

void UVMSkillsCooldownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BasicSkillCooldown = 0.0f;
	AdvancedSkillCooldown = 0.0f;
	MovementSkillCooldown = 0.0f;
	UltimateSkillCooldown = 0.0f;
}

void UVMSkillsCooldownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (BasicSkillCooldown > KINDA_SMALL_NUMBER) BasicSkillCooldown -= InDeltaTime;
	if (AdvancedSkillCooldown > KINDA_SMALL_NUMBER) AdvancedSkillCooldown -= InDeltaTime;
	if (MovementSkillCooldown > KINDA_SMALL_NUMBER) MovementSkillCooldown -= InDeltaTime;
	if (UltimateSkillCooldown > KINDA_SMALL_NUMBER) UltimateSkillCooldown -= InDeltaTime;

	if (BasicSkillCooldown < KINDA_SMALL_NUMBER)
	{
		BasicSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT(" "))));
	}
	else
	{
		BasicSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), BasicSkillCooldown)));
	}

	if (AdvancedSkillCooldown < KINDA_SMALL_NUMBER)
	{
		AdvancedSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT(" "))));
	}
	else
	{
		AdvancedSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), BasicSkillCooldown)));
	}

	if (MovementSkillCooldown < KINDA_SMALL_NUMBER)
	{
		MovementSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT(" "))));
	}
	else
	{
		MovementSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), BasicSkillCooldown)));
	}

	if (UltimateSkillCooldown < KINDA_SMALL_NUMBER)
	{
		UltimateSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT(" "))));
	}
	else
	{
		UltimateSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.1f sec"), BasicSkillCooldown)));
	}
}

void UVMSkillsCooldownWidget::Cooldown(float DeltaTime)
{
}
