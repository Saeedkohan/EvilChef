// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h" // برای استفاده از UDataAsset
#include "FinalOrderDetail.generated.h"

class UMenuItemDataAsset;
class UExtraDataAsset;
class UCompanionDataAsset;

USTRUCT(BlueprintType)
struct FFinalOrderDetail
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Order")
	UMenuItemDataAsset* Item = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Order")
	TArray<UExtraDataAsset*> Extras;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Order")
	TArray<UCompanionDataAsset*> Companions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order")
	FString Intro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Order")
	FString Outro;
};
