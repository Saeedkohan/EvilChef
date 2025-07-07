// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DialogueDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class EVILCHEF_API UDialogueDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Dialogue")
	TMap<FString,FString> DialogueEntries;
};

