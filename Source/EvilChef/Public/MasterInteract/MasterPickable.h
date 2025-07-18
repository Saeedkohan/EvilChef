// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MasterInteract/MasterInteract.h"
#include "MasterPickable.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
UCLASS()
class EVILCHEF_API AMasterPickable : public AMasterInteract
{
	GENERATED_BODY()


public:
	
	AMasterPickable();

	virtual void Interact_Implementation() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UStaticMeshComponent* GetItemMesh() const { return ItemMesh; }
};
