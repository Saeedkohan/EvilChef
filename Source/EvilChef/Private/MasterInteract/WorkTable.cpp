// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterInteract/WorkTable.h"

#include "EvilChefCharacter.h"
#include "Kismet/GameplayStatics.h"

AWorkTable::AWorkTable()
{
	TableMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TableMesh"));
	
}

void AWorkTable::Interact_Implementation()
{
	Super::Interact_Implementation();
	AEvilChefCharacter* Player=Cast<AEvilChefCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
	if (Player && Player->GetHeldItem())
	{
		Player->PlaceItem(GetActorLocation() + FVector(0,0,50));
	}
}
