// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/ExtraItemWidget.h"

#include "Components/TextBlock.h"

void UExtraItemWidget::SetExtraName(const FText& InName)
{
	if (ExtraNameText)
	{
		ExtraNameText->SetText(InName);
	}
}

void UExtraItemWidget::SetItemNameText(const FText& InText)
{
	if (ItemName)
	{
		ItemName->SetText(InText);
	}
}
