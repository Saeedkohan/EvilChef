// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Dialogue/CustomerBPI.h"
#include "Dialogue/FinalOrderDetail.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MasterInteract/InteractBPI.h"
#include "EvilChefCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;
class AMasterPickable;
class AMasterInteract;
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AEvilChefCharacter : public ACharacter, public ICustomerBPI, public IInteractBPI
{
	GENERATED_BODY()

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

protected:
	/** Jump Input Action */


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Input")
	class UInputAction* MouseLookAction;



public:
	AEvilChefCharacter();

protected:
	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

protected:
	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:
	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UMainHudWidget> MainHudClass;


	UPROPERTY()
	class UMainHudWidget* MainHudWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	AMasterPickable* HeldItem;

public:
	virtual void SendTheOrder_Implementation(const FFinalOrderDetail& OrderData) override;

	virtual void SendInteractReference_Implementation(AActor* InteractableActor) override;

	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AActor* CurrentInteractable;


	void PickupItem(AMasterPickable* ItemToPickup);
	void PlaceItem(const FVector& NewLocation);
	void Tick(float DeltaTime);

	AMasterPickable* GetHeldItem() const { return HeldItem; }


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* GhostMeshComponent;

protected:
	void OnInteract();
	
	 UPROPERTY(EditDefaultsOnly, Category="Interaction")
        float InteractionDotThreshold = 0.8f; 
	// virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
public:
	UPROPERTY(BlueprintReadOnly, Category = "Interaction")
	AMasterInteract* FocusedInteractable;
	void PlaceItem();


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Placement")
	UMaterialInterface* GhostMaterial_Green;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction|Placement")
	UMaterialInterface* GhostMaterial_Red;

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float InteractionDistance = 300.0f;
	bool bIsPlacementModeActive;
};
