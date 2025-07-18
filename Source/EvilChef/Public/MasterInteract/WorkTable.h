// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterInteract/MasterInteract.h"
#include "WorkTable.generated.h"

/**
 * 
 */

class UMaterialInterface;

UCLASS()


class EVILCHEF_API AWorkTable : public AMasterInteract
{
	GENERATED_BODY()

public:
	AWorkTable();
	virtual void Interact_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* TableMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	UMaterialInterface* GhostMaterial;
};
