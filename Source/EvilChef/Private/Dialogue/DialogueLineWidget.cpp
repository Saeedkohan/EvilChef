// DialogueLineWidget.cpp
#include "Dialogue/DialogueLineWidget.h"

#include "MenuItemDataAsset.h"
#include "Components/TextBlock.h"
#include "Dialogue/FinalOrderDetail.h"
#include "Extra//ExtraDataAsset.h"
#include "Companion//CompanionDataAsset.h"

void UDialogueLineWidget::UpdateWidget(const FFinalOrderDetail& OrderDetail)
{

	if (IntroText) { IntroText->SetText(FText::FromString(OrderDetail.Intro)); }
	if (OutroText) { OutroText->SetText(FText::FromString(OrderDetail.Outro)); }
	if (ItemNameText && OrderDetail.Item) { ItemNameText->SetText(OrderDetail.Item->ItemName); }

	
	if (ExtrasText)
	{
		if (OrderDetail.Extras.Num() > 0)
		{
			FString ExtrasString = TEXT(" with "); 
			for (int i = 0; i < OrderDetail.Extras.Num(); ++i)
			{
				ExtrasString += OrderDetail.Extras[i]->ItemName.ToString();
				if (i < OrderDetail.Extras.Num() - 1)
				{
					ExtrasString += TEXT(" and ");
				}
			}
			ExtrasText->SetText(FText::FromString(ExtrasString));
			ExtrasText->SetVisibility(ESlateVisibility::HitTestInvisible); 
		}
		else
		{
			
			ExtrasText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	
	if (CompanionsText)
	{
		if (OrderDetail.Companions.Num() > 0)
		{
			FString CompanionsString = TEXT(" and a "); 
			for (int i = 0; i < OrderDetail.Companions.Num(); ++i)
			{
				CompanionsString += OrderDetail.Companions[i]->ItemName.ToString();
				if (i < OrderDetail.Companions.Num() - 1)
				{
					CompanionsString += TEXT(" and ");
				}
			}
			CompanionsText->SetText(FText::FromString(CompanionsString));
			CompanionsText->SetVisibility(ESlateVisibility::HitTestInvisible);
		}
		else
		{
			
			CompanionsText->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
}