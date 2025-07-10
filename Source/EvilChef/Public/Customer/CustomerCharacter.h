// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Dialogue/ECustomerGender.h"
#include "Dialogue/FinalOrderDetail.h"
#include "CustomerCharacter.generated.h"

class UDialogueDataAsset;
class UMenuItemDataAsset;
class UDialogueItemDataAsset;;

UCLASS()
class EVILCHEF_API ACustomerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Customer | Config")
	TArray<UMenuItemDataAsset*>MenuItems;
	
	UPROPERTY(EditDefaultsOnly,Category="Customer | Config")
	TArray<UDialogueDataAsset*>DialogueStyles;

	UPROPERTY(EditDefaultsOnly,Category="Customer | Config")
	TArray<USkeletalMesh*>MaleMeshes;

	UPROPERTY(EditDefaultsOnly,Category="Customer | Config")
	TArray<USkeletalMesh*>FemaleMeshes;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Customer | State")
	ECustomerGender Gender;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Customer | State")
	FFinalOrderDetail FinalOrder;
	
	UFUNCTION(BlueprintCallable)
	void GenerateCustomer();
	UFUNCTION(BlueprintCallable)
	void GenerateOrder();


	UFUNCTION()
	void OnReadyToSendOrder();

	FTimerHandle OrderSendDelayTimer;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
