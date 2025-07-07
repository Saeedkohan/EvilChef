// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExtraItemWidget.generated.h"

/**
 * 
 */
class UTextBlock;
UCLASS()
class EVILCHEF_API UExtraItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetExtraName(const FText& InName);
	void SetItemNameText(const FText& InText);
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ExtraNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName; 
	
};
