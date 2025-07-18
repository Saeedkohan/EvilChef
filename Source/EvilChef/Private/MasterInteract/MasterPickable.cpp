#include "MasterInteract//MasterPickable.h"
#include "Components/StaticMeshComponent.h"
#include "EvilChefCharacter.h" 
#include "Kismet/GameplayStatics.h"

AMasterPickable::AMasterPickable()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetupAttachment(RootComponent);


	ItemMesh->ComponentTags.Add(FName("mesh"));
}

void AMasterPickable::Interact_Implementation()
{
	Super::Interact_Implementation();
	UE_LOG(LogTemp, Warning, TEXT("MasterPickable's Interact function was called on: %s"), *GetName());

	AEvilChefCharacter* Player = Cast<AEvilChefCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (Player)
	{
		Player->PickupItem(this);
	}
}