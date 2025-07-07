// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHudWidget.generated.h"

/**
 * 
 */
class  UVerticalBox;
class UDialogueLineWidget;
UCLASS()
class EVILCHEF_API UMainHudWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void AddDialogueLine(const FText& DialogueText);
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* DialogueContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Dialogue")
	TSubclassOf<UDialogueLineWidget> DialogueLineClass;
	
};
