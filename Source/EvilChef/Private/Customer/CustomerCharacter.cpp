// Fill out your copyright notice in the Description page of Project Settings.


#include "Customer/CustomerCharacter.h"

#include "MenuItemDataAsset.h"
#include "Dialogue/CustomerBPI.h"
#include "Dialogue/DialogueDataAsset.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACustomerCharacter::ACustomerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ACustomerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GenerateCustomer();
	GenerateOrder();

	GetWorld()->GetTimerManager().SetTimer(OrderSendDelayTimer, this, &ACustomerCharacter::OnReadyToSendOrder, 1.0f,
	                                       false);
}

void ACustomerCharacter::GenerateCustomer()
{
	Gender = (FMath::RandBool()) ? ECustomerGender::Male : ECustomerGender::Female;
	// بر اساس جنسیت، یک مش تصادفی انتخاب و اعمال کن
	if (Gender == ECustomerGender::Male && MaleMeshes.Num() > 0)
	{
		GetMesh()->SetSkeletalMesh(MaleMeshes[FMath::RandRange(0, MaleMeshes.Num() - 1)]);
	}
	else if (Gender == ECustomerGender::Female && FemaleMeshes.Num() > 0)
	{
		GetMesh()->SetSkeletalMesh(FemaleMeshes[FMath::RandRange(0, FemaleMeshes.Num() - 1)]);
	}
}

void ACustomerCharacter::GenerateOrder()
{
	if (MenuItems.Num() == 0 || DialogueStyles.Num() == 0) return;

	FinalOrder.Item = MenuItems[FMath::RandRange(0, MenuItems.Num() - 1)];

	if (FinalOrder.Item->CompatibleExtras.Num() > 0)
	{
		FinalOrder.Extras.Add(
			FinalOrder.Item->CompatibleExtras[FMath::RandRange(0, FinalOrder.Item->CompatibleExtras.Num() - 1)]);
	}
	if (FinalOrder.Item->CompatibleCompanions.Num() > 0)
	{
		FinalOrder.Companions.Add(
			FinalOrder.Item->CompatibleCompanions[FMath::RandRange(0, FinalOrder.Item->CompatibleCompanions.Num() - 1)]);
	}

	UDialogueDataAsset* ChosenDialogueStyle = DialogueStyles[FMath::RandRange(0, DialogueStyles.Num() - 1)];
	if (ChosenDialogueStyle && ChosenDialogueStyle->DialogueEntries.Num() > 0)
	{
		TArray<FString> Keys;
		ChosenDialogueStyle->DialogueEntries.GenerateKeyArray(Keys);
		int32 RandomIndex = FMath::RandRange(0, Keys.Num() - 1);

		FinalOrder.Intro = Keys[RandomIndex];
		FinalOrder.Outro = ChosenDialogueStyle->DialogueEntries[Keys[RandomIndex]];
	}
}

void ACustomerCharacter::OnReadyToSendOrder()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	if (PlayerCharacter && PlayerCharacter->GetClass()->ImplementsInterface(UCustomerBPI::StaticClass()))
	{
		ICustomerBPI::Execute_SendTheOrder(PlayerCharacter, FinalOrder);
	}
}

// Called every frame
void ACustomerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACustomerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
