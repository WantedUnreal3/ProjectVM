// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Skill/VMSkillsCooldownWidget.h"

#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Math/UnitConversion.h"
#include "Hero/SkillBase.h"

void UVMSkillsCooldownWidget::StartBasicSkillCooldown(USkillBase* Skill)
{
	BasicSkillCooldownBase = Skill->GetCooldown();
	BasicSkillCooldownRemaining = BasicSkillCooldownBase;
	BasicSkillCooldownProgressBar->SetPercent(0.0f);
	BasicSkillIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
	BasicSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), BasicSkillCooldownRemaining)));
}

void UVMSkillsCooldownWidget::StartAdvancedSkillCooldown(USkillBase* Skill)
{
	AdvancedSkillCooldownBase = Skill->GetCooldown();
	AdvancedSkillCooldownRemaining = AdvancedSkillCooldownBase;
	AdvancedSkillCooldownProgressBar->SetPercent(0.0f);
	AdvancedSkillIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
	AdvancedSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), AdvancedSkillCooldownRemaining)));
}

void UVMSkillsCooldownWidget::StartMovementSkillCooldown(USkillBase* Skill)
{
	MovementSkillCooldownBase = Skill->GetCooldown();
	MovementSkillCooldownRemaining = MovementSkillCooldownBase;
	MovementSkillCooldownProgressBar->SetPercent(0.0f);
	MovementSkillIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
	MovementSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), MovementSkillCooldownRemaining)));
}

void UVMSkillsCooldownWidget::StartUltimateSkillCooldown(USkillBase* Skill)
{
	UltimateSkillCooldownBase = Skill->GetCooldown();
	UltimateSkillCooldownRemaining = UltimateSkillCooldownBase;
	UltimateSkillCooldownProgressBar->SetPercent(0.0f);
	UltimateSkillIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.5f));
	UltimateSkillCooldownText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), UltimateSkillCooldownRemaining)));
}

void UVMSkillsCooldownWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BasicSkillCooldownProgressBar->SetPercent(0.0f);
	AdvancedSkillCooldownProgressBar->SetPercent(0.0f);
	MovementSkillCooldownProgressBar->SetPercent(0.0f);
	UltimateSkillCooldownProgressBar->SetPercent(0.0f);

	BasicSkillCooldownText->SetText(FText::FromString(TEXT("")));
	AdvancedSkillCooldownText->SetText(FText::FromString(TEXT("")));
	MovementSkillCooldownText->SetText(FText::FromString(TEXT("")));
	UltimateSkillCooldownText->SetText(FText::FromString(TEXT("")));
}

void UVMSkillsCooldownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);


	if (BasicSkillCooldownRemaining > 0)
	{
		UpdateBasicSkillCooldown(InDeltaTime);
	}

	if (AdvancedSkillCooldownRemaining > 0)
	{
		UpdateAdvancedSkillCooldown(InDeltaTime);
	}

	if (MovementSkillCooldownRemaining > 0)
	{
		UpdateMovementSkillCooldown(InDeltaTime);
	}

	if (UltimateSkillCooldownRemaining > 0)
	{
		UpdateUltimateSkillCooldown(InDeltaTime);
	}
}

void UVMSkillsCooldownWidget::UpdateBasicSkillCooldown(float DeltaTime)
{
	BasicSkillCooldownRemaining -= DeltaTime;
	
	if (BasicSkillCooldownRemaining <= 0)
	{
		BasicSkillCooldownProgressBar->SetPercent(0.0f);
		BasicSkillIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		BasicSkillCooldownText->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		BasicSkillCooldownProgressBar->SetPercent(1.0f - BasicSkillCooldownRemaining / BasicSkillCooldownBase);
		int32 CooldownSeconds = FMath::CeilToInt(BasicSkillCooldownRemaining);
		BasicSkillCooldownText->SetText(FText::FromString(FString::FromInt(CooldownSeconds)));
	}
}

void UVMSkillsCooldownWidget::UpdateAdvancedSkillCooldown(float DeltaTime)
{
	AdvancedSkillCooldownRemaining -= DeltaTime;
	
	if (AdvancedSkillCooldownRemaining <= 0)
	{
		AdvancedSkillCooldownProgressBar->SetPercent(0.0f);
		AdvancedSkillIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		AdvancedSkillCooldownText->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		AdvancedSkillCooldownProgressBar->SetPercent(1.0f - AdvancedSkillCooldownRemaining / AdvancedSkillCooldownBase);
		int32 CooldownSeconds = FMath::CeilToInt(AdvancedSkillCooldownRemaining);
		AdvancedSkillCooldownText->SetText(FText::FromString(FString::FromInt(CooldownSeconds)));
	}
}

void UVMSkillsCooldownWidget::UpdateMovementSkillCooldown(float DeltaTime)
{
	MovementSkillCooldownRemaining -= DeltaTime;
	
	if (MovementSkillCooldownRemaining <= 0)
	{
		MovementSkillCooldownProgressBar->SetPercent(0.0f);
		MovementSkillIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		MovementSkillCooldownText->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		MovementSkillCooldownProgressBar->SetPercent(1.0f - MovementSkillCooldownRemaining / MovementSkillCooldownBase);
		int32 CooldownSeconds = FMath::CeilToInt(MovementSkillCooldownRemaining);
		MovementSkillCooldownText->SetText(FText::FromString(FString::FromInt(CooldownSeconds)));
	}
}

void UVMSkillsCooldownWidget::UpdateUltimateSkillCooldown(float DeltaTime)
{
	UltimateSkillCooldownRemaining -= DeltaTime;
	
	if (UltimateSkillCooldownRemaining <= 0)
	{
		UltimateSkillCooldownProgressBar->SetPercent(0.0f);
		UltimateSkillIcon->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
		UltimateSkillCooldownText->SetText(FText::FromString(TEXT("")));
	}
	else
	{
		UltimateSkillCooldownProgressBar->SetPercent(1.0f - UltimateSkillCooldownRemaining / UltimateSkillCooldownBase);
		int32 CooldownSeconds = FMath::CeilToInt(UltimateSkillCooldownRemaining);
		UltimateSkillCooldownText->SetText(FText::FromString(FString::FromInt(CooldownSeconds)));
	}
}