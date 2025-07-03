// Copyright Epic Games, Inc. All Rights Reserved.


#include "EvilChefPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "EvilChefCameraManager.h"

AEvilChefPlayerController::AEvilChefPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AEvilChefCameraManager::StaticClass();
}

void AEvilChefPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
