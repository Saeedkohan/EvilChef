// DialogueLineWidget.cpp
#include "Dialogue/DialogueLineWidget.h"
#include "Components/TextBlock.h"

void UDialogueLineWidget::SetDialogueText(const FText& InText)
{
	if (DialogueText)
	{
		DialogueText->SetText(InText);
	}
}

void UDialogueLineWidget::SetItemNameText(const FText& InText)
{
	if (ItemName)
	{
		ItemName->SetText(InText);
	}
}