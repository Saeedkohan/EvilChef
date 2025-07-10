// Fill out your copyright notice in the Description page of Project Settings.


#include "Dialogue/MainHudWidget.h"

#include "Components/VerticalBox.h"
#include "Dialogue/DialogueLineWidget.h"

void UMainHudWidget::DisplayOrder(const FFinalOrderDetail& OrderDetail)
{
	if (DialogueLine)
	{
		DialogueLine->UpdateWidget(OrderDetail);
		DialogueLine->SetVisibility(ESlateVisibility::HitTestInvisible);
	}
}