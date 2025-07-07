// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/MainHudWidget.h"

#include "Components/VerticalBox.h"
#include "Dialogue/DialogueLineWidget.h"

void UMainHudWidget::AddDialogueLine(const FText& DialogueText)
{
	if (DialogueLineClass && DialogueContainer)
	{
		UDialogueLineWidget* NewLineWidget = CreateWidget<UDialogueLineWidget>(this, DialogueLineClass);
		if (NewLineWidget)
		{
			NewLineWidget->SetDialogueText(DialogueText);
			DialogueContainer->AddChildToVerticalBox(NewLineWidget);
		}
	}
}
