// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractBPI.h" 
#include "Components/SceneComponent.h" 
#include "MasterInteract.generated.h"

class UBoxComponent;
class UWidgetComponent;
class UMasterDataAsset;
class AEvilChefCharacter;

UCLASS()
class EVILCHEF_API AMasterInteract : public AActor ,public IInteractBPI
{
	GENERATED_BODY()
	
public:	
	AMasterInteract();

	void Interact_Implementation();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWidgetComponent* ItemNameWidget;
	
	
	UPROPERTY(EditAnywhere , BlueprintReadOnly, Category = "Config")
	UMasterDataAsset* ItemData;

	
	UPROPERTY()
	AEvilChefCharacter* PlayerCharacterRef;

	
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float InteractionDotThreshold = 0.92f;

	
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UFUNCTION(BlueprintCallable)
	bool PlayerHasItem();
public:
	UFUNCTION(BlueprintCallable)
	void HideWidget();

	UFUNCTION(BlueprintCallable)
	void ShowWidget();


	
};