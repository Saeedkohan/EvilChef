// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterInteract/MasterInteract.h"

#include "EvilChefCharacter.h"
#include "MasterDataAsset.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MasterInteract/InteractBPI.h"
#include "MasterInteract/ItemNameWidget.h"

AMasterInteract::AMasterInteract()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false; 

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	ItemNameWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("ItemNameWidget"));
	ItemNameWidget->SetupAttachment(RootComponent);
	ItemNameWidget->SetWidgetSpace(EWidgetSpace::Screen); 
	ItemNameWidget->SetVisibility(false); 
}

void AMasterInteract::BeginPlay()
{
	Super::BeginPlay();


	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AMasterInteract::OnBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &AMasterInteract::OnEndOverlap);


	PlayerCharacterRef = Cast<AEvilChefCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	
	if (ItemData)
	{
		UItemNameWidget* NameWidget = Cast<UItemNameWidget>(ItemNameWidget->GetUserWidgetObject());
		if (NameWidget)
		{
			NameWidget->SetItemName(ItemData->ItemName);
		}
	}
}

void AMasterInteract::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerCharacterRef) return;

	
	FVector PlayerCameraLocation = PlayerCharacterRef->GetFirstPersonCameraComponent()->GetComponentLocation();
	FVector DirectionToActor = GetActorLocation() - PlayerCameraLocation;
	DirectionToActor.Normalize();

	FVector CameraForward = PlayerCharacterRef->GetFirstPersonCameraComponent()->GetForwardVector();

	float DotProduct = FVector::DotProduct(DirectionToActor, CameraForward);

	if (DotProduct > InteractionDotThreshold)
	{
		
		if (PlayerCharacterRef->GetClass()->ImplementsInterface(UInteractBPI::StaticClass()))
		{
			IInteractBPI::Execute_SendInteractReference(PlayerCharacterRef, this);
		}
	}
}

void AMasterInteract::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == PlayerCharacterRef)
	{
		ShowWidget();
		SetActorTickEnabled(true); 
	}
}

void AMasterInteract::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == PlayerCharacterRef)
	{
		HideWidget();
		SetActorTickEnabled(false); 
		
		
		if (PlayerCharacterRef->GetClass()->ImplementsInterface(UInteractBPI::StaticClass()))
		{
			IInteractBPI::Execute_SendInteractReference(PlayerCharacterRef, nullptr);
		}
	}
}


bool AMasterInteract::PlayerHasItem()
{
	// TODO: Implement inventory check logic here
	return false; 
}

void AMasterInteract::HideWidget(){ ItemNameWidget->SetVisibility(false); }
void AMasterInteract::ShowWidget(){ ItemNameWidget->SetVisibility(true); }
