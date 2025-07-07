// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/CompanionItemWidget.h"

#include "Components/TextBlock.h"

void UCompanionItemWidget::SetCompanionName(const FText& InName)
{
	if (CompanionNameText)
	{
		CompanionNameText->SetText(InName);
	}
}

void UCompanionItemWidget::SetItemNameText(const FText& InText)
{
	if (ItemName)
	{
		ItemName->SetText(InText);
	}
}
