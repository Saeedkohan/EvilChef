// Copyright Epic Games, Inc. All Rights Reserved.

#include "EvilChefCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Dialogue/MainHudWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MasterInteract/MasterPickable.h"
#include "MasterInteract//WorkTable.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AEvilChefCharacter

AEvilChefCharacter::AEvilChefCharacter()
{


	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f),
	                                                           FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	AttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("AttachPoint"));
	AttachPoint->SetupAttachment(GetFirstPersonCameraComponent());
	AttachPoint->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f));


	GhostMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GhostMesh"));
	GhostMeshComponent->SetupAttachment(RootComponent);
	GhostMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	GhostMeshComponent->SetVisibility(false); 
}

void AEvilChefCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AEvilChefCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AEvilChefCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AEvilChefCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AEvilChefCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &AEvilChefCharacter::LookInput);

		PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AEvilChefCharacter::OnInteract);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}


void AEvilChefCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AEvilChefCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void AEvilChefCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AEvilChefCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AEvilChefCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AEvilChefCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}


void AEvilChefCharacter::BeginPlay()
{
	Super::BeginPlay();


	if (MainHudClass)
	{
		MainHudWidget = CreateWidget<UMainHudWidget>(GetWorld(), MainHudClass);


		if (MainHudWidget)
		{
			MainHudWidget->AddToViewport();
		}
	}
}


void AEvilChefCharacter::SendTheOrder_Implementation(const FFinalOrderDetail& OrderData)
{
	if (MainHudWidget)
	{
		MainHudWidget->DisplayOrder(OrderData);
	}
}

void AEvilChefCharacter::SendInteractReference_Implementation(AActor* InteractableActor)
{
	CurrentInteractable = InteractableActor;
}

void AEvilChefCharacter::OnInteract()
{
	UE_LOG(LogTemp, Warning, TEXT("OnInteract called. FocusedInteractable is: %s"), *GetNameSafe(FocusedInteractable));
	if (FocusedInteractable)
	{
		if (FocusedInteractable->GetClass()->ImplementsInterface(UInteractBPI::StaticClass()))
		{
			IInteractBPI::Execute_Interact(FocusedInteractable);
		}
	}
}

void AEvilChefCharacter::PickupItem(AMasterPickable* ItemToPickup)
{
	// لاگ شماره ۳: چک می‌کند آیا تابع برداشتن آیتم صدا زده شده یا نه
	UE_LOG(LogTemp, Warning, TEXT("PickupItem function called with item: %s"), *GetNameSafe(ItemToPickup));

	if (HeldItem) { return; } // اگر آیتمی در دست داریم، فعلا آیتم جدیدی برنمی‌داریم

	if (ItemToPickup)
	{
		HeldItem = ItemToPickup;

		// غیرفعال کردن فیزیک و برخورد
		HeldItem->ItemMesh->SetSimulatePhysics(false);
		HeldItem->SetActorEnableCollision(false);

		// اتصال به نقطه مورد نظر
		HeldItem->AttachToComponent(AttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		
		UE_LOG(LogTemp, Warning, TEXT("Successfully attached %s!"), *ItemToPickup->GetName());
	}
}

void AEvilChefCharacter::PlaceItem(const FVector& NewLocation)
{
	if (HeldItem)
	{
		HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HeldItem->SetActorLocation(NewLocation);
		HeldItem->SetActorEnableCollision(true);
		HeldItem = nullptr;
	}
}



// EvilChefCharacter.cpp

void AEvilChefCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // لاگ شماره ۱: برای اطمینان از اینکه تابع Tick اصلاً اجرا می‌شود
    UE_LOG(LogTemp, Warning, TEXT("Player Tick is Running..."));

    // یک لیست از تمام آیتم‌هایی که در محدوده آنها هستیم، تهیه کن
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, AMasterInteract::StaticClass());

    // لاگ شماره ۲: برای دیدن تعداد آیتم‌های پیدا شده در محدوده
    UE_LOG(LogTemp, Warning, TEXT("Found %d Overlapping Interactables."), OverlappingActors.Num());

    AMasterInteract* BestInteractable = nullptr;
    float BestDot = -1.0f; 

    // در میان تمام آیتم‌های نزدیک، بهترین گزینه را پیدا کن
    for (AActor* OverlappedActor : OverlappingActors)
    {
        AMasterInteract* Interactable = Cast<AMasterInteract>(OverlappedActor);
        if (Interactable)
        {
            // محاسبه زاویه دید
            FVector PlayerCameraLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
            FVector DirectionToActor = Interactable->GetActorLocation() - PlayerCameraLocation;
            DirectionToActor.Normalize();
            float DotProduct = FVector::DotProduct(GetFirstPersonCameraComponent()->GetForwardVector(), DirectionToActor);

            // لاگ شماره ۳: برای دیدن مقدار زاویه دید برای هر آیتم
            UE_LOG(LogTemp, Warning, TEXT("Checking item: %s - Dot Product is: %f"), *Interactable->GetName(), DotProduct);

            if (DotProduct > InteractionDotThreshold && DotProduct > BestDot)
            {
                BestDot = DotProduct;
                BestInteractable = Interactable;
            }
        }
    }

    // مدیریت نمایش ویجت‌ها
    if (FocusedInteractable != BestInteractable)
    {
        if (FocusedInteractable)
        {
            FocusedInteractable->HideWidget();
        }

        if (BestInteractable)
        {
            BestInteractable->ShowWidget();
        }

        FocusedInteractable = BestInteractable;
    }


	
	if (HeldItem) 
	{
		FVector TraceStart = GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector TraceEnd = TraceStart + (GetFirstPersonCameraComponent()->GetForwardVector() * InteractionDistance);
		FHitResult HitResult;

		// یک اشعه از دوربین شلیک کن
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility))
		{
			// چک کن آیا به یک میز برخورد کرده‌ایم یا نه
			AWorkTable* TargetTable = Cast<AWorkTable>(HitResult.GetActor());

			// مش و متریال مناسب را تنظیم کن
			GhostMeshComponent->SetStaticMesh(HeldItem->GetItemMesh()->GetStaticMesh());
			GhostMeshComponent->SetMaterial(0, TargetTable ? GhostMaterial_Green : GhostMaterial_Red);

			// روح را در محل برخورد قرار بده و آن را قابل مشاهده کن
			GhostMeshComponent->SetWorldLocation(HitResult.Location);
			GhostMeshComponent->SetVisibility(true);
		}
		else
		{
			// اگر اشعه به جایی برخورد نکرد، روح را مخفی کن
			GhostMeshComponent->SetVisibility(false);
		}
	}
	else
	{
		// اگر آیتمی در دست نداریم، روح را مخفی کن
		GhostMeshComponent->SetVisibility(false);
	}
}