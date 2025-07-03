// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MasterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class EVILCHEF_API UMasterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Details")
	FText ItemName;
	
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Details")
	// UTexture2D* ItemIcon;
};
