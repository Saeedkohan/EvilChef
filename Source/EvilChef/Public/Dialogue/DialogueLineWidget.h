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

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateWidget(const FFinalOrderDetail& OrderDetail);

protected:
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* IntroText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExtrasText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CompanionsText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* OutroText;
};