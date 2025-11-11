#include "VMQuestTrackerWidget.h"
#include "VMQuestDataObject.h"
#include "Components/TextBlock.h"

void UVMQuestTrackerWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{

	QuestDataObject = Cast<UVMQuestDataObject>(ListItemObject);
	if (QuestDataObject == nullptr)
	{
		return;
	}

	//구독 등록
	QuestDataObject->OnOptionUpdated.AddUObject(this, &UVMQuestTrackerWidget::UpdateUI);

	UpdateUI();
}

void UVMQuestTrackerWidget::UpdateUI()
{
	if (QuestDataObject == nullptr)
	{
		return;
	}

	FVMQuestData Data = QuestDataObject->QuestData;

	if (QuestName != nullptr)
	{
		QuestName->SetText(Data.QuestTitle);
	}
	if (QuestSummary != nullptr)
	{
		FText QuestText = FText::Format(
			FText::FromString(TEXT("{0} ({1}/{2})")),
			FText::FromName(Data.QuestTarget),
			FText::AsNumber(QuestDataObject->CurrentCount),
			FText::AsNumber(Data.TargetCount)
		);
		QuestSummary->SetText(QuestText);
	}
}
