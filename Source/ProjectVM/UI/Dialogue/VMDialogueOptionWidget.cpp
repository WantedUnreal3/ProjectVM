#include "UI/Dialogue/VMDialogueOptionWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "VMDialogueOption.h"
#include "NPC/VMNPC.h"
#include "NPC/VMNPCEnums.h"

void UVMDialogueOptionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (OptionButton != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OptionButton is Succed"));
		//버튼 클릭 이벤트 바인딩 
		OptionButton->OnClicked.AddDynamic(this, &UVMDialogueOptionWidget::OnOptionButtonClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OptionButton is null in UVMDialogueOptionWidget"));
	}
}

void UVMDialogueOptionWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	Option = Cast<UVMDialogueOption>(ListItemObject);
	if (Option != nullptr)
	{
		if (OptionTextBlock != nullptr)
		{
			OptionTextBlock->SetText(FText::FromString(Option->OptionText));
		}
	}
}

void UVMDialogueOptionWidget::OnOptionButtonClicked()
{
	//버튼 클릭 시 처리할 로직 작성
	UE_LOG(LogTemp, Log, TEXT("Dialogue Option Button Clicked: %s"), *OptionTextBlock->GetText().ToString());
	UE_LOG(LogTemp, Log, TEXT("Dialogue Option Button Clicked: %s"), *Option->OwnerNPC->GetName());
	switch (Option->OptionType)
	{
	case ENPCOption::Talk:
		Option->OwnerNPC->StartDailyTalk();
		break;
	case ENPCOption::Quest:
		Option->OwnerNPC->StartQuest();
		break;
	case ENPCOption::QuestClear:
		Option->OwnerNPC->QuestCompleted();
	case ENPCOption::Exit:
		Option->OwnerNPC->EndDialogue();
		break;
	case ENPCOption::Accept:
		Option->OwnerNPC->AcceptQuest();
		break;
	case ENPCOption::Decline:
		Option->OwnerNPC->EndDialogue();
		break;
	default:
		break;
	}
}
