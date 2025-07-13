// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterInteract/ItemNameWidget.h"

#include "Components/TextBlock.h"

void UItemNameWidget::SetItemName(const FText& InmName)
{
	if (ItemNameText)
	{
		ItemNameText->SetText(InmName);
	}
}
