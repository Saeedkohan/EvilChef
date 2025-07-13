// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemNameWidget.generated.h"

class UTextBlock;

UCLASS()
class EVILCHEF_API UItemNameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetItemName(const FText& InName);
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;
};
