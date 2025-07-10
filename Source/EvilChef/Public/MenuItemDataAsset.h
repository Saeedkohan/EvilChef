// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterDataAsset.h"
#include "MenuItemDataAsset.generated.h"

class UCompanionDataAsset;
class UExtraDataAsset;

UCLASS()
class EVILCHEF_API UMenuItemDataAsset : public UMasterDataAsset
{
	GENERATED_BODY()

public:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item Details")
	// float Price;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Compatibility")
	TArray<UCompanionDataAsset*> CompatibleCompanions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Compatibility")
	TArray<UExtraDataAsset*> CompatibleExtras;



};
