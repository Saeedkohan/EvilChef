// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHudWidget.generated.h"

class UDialogueLineWidget;
struct FFinalOrderDetail;

UCLASS()
class EVILCHEF_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void DisplayOrder(const FFinalOrderDetail& OrderDetail);
	
protected:

	UPROPERTY(meta = (BindWidget))
	UDialogueLineWidget* DialogueLine;
};