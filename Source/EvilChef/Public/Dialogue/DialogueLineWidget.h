// DialogueLineWidget.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogueLineWidget.generated.h"

class UTextBlock;

UCLASS()
class EVILCHEF_API UDialogueLineWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDialogueText(const FText& InText);
	void SetItemNameText(const FText& InText);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DialogueText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName; 
};