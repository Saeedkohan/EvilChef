// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CompanionItemWidget.generated.h"

/**
 * 
 */

class UTextBlock;
UCLASS()
class EVILCHEF_API UCompanionItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCompanionName(const FText& InName);
	void SetItemNameText(const FText& InText);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CompanionNameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemName; 
};
