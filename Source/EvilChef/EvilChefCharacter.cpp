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
#include "DrawDebugHelpers.h"
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


	bIsPlacementModeActive = false;
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

// EvilChefCharacter.cpp

void AEvilChefCharacter::OnInteract()
{
	// --- حالت اول: بازیکن آیتمی در دست ندارد ---
	if (!HeldItem)
	{
		if (FocusedInteractable)
		{
			// چک کن آیا آیتم قابل برداشتن است یا نه
			if (AMasterPickable* PickableItem = Cast<AMasterPickable>(FocusedInteractable))
			{
				PickupItem(PickableItem);
			}
			// در آینده می‌توانید تعامل با چیزهای دیگر (مثل دکمه) را اینجا اضافه کنید
		}
		return; // بعد از تلاش برای برداشتن، از تابع خارج شو
	}

	// --- حالت دوم: بازیکن آیتمی در دست دارد ---
	if (HeldItem)
	{
		// اگر در حالت گذاشتن نیستیم، با فشار F وارد این حالت شو
		if (!bIsPlacementModeActive)
		{
			bIsPlacementModeActive = true;
			UE_LOG(LogTemp, Warning, TEXT("Placement Mode Activated."));
			return;
		}

		// اگر در حالت گذاشتن هستیم، با فشار F آیتم را بگذار
		if (bIsPlacementModeActive)
		{
			// چک کن که آیا Ghost Mesh قابل مشاهده و سبز رنگ است
			if (GhostMeshComponent->IsVisible() && GhostMeshComponent->GetMaterial(0) == GhostMaterial_Green)
			{
				PlaceItem();
			}
			else
			{
				// اگر محل نامناسب بود، حالت گذاشتن را لغو کن
				bIsPlacementModeActive = false;
				GhostMeshComponent->SetVisibility(false);
				UE_LOG(LogTemp, Warning, TEXT("Cannot place item here. Deactivating placement mode."));
			}
		}
	}
}

// EvilChefCharacter.cpp

void AEvilChefCharacter::PickupItem(AMasterPickable* ItemToPickup)
{
	if (HeldItem || !ItemToPickup) { return; }

	HeldItem = ItemToPickup;
	HeldItem->ItemMesh->SetSimulatePhysics(false);
	HeldItem->SetActorEnableCollision(false);
	HeldItem->AttachToComponent(AttachPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// ===== خط جدید =====
	// وقتی آیتم جدیدی برمی‌داریم، حالت گذاشتن را ریست کن
	bIsPlacementModeActive = false; 
	
	UE_LOG(LogTemp, Warning, TEXT("Successfully picked up %s!"), *ItemToPickup->GetName());
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

    // بخش ۱: پیدا کردن آیتمی که به آن نگاه می‌کنیم (این بخش درست است)
    TArray<AActor*> OverlappingActors;
    GetOverlappingActors(OverlappingActors, AMasterInteract::StaticClass());

    AMasterInteract* BestInteractable = nullptr;
    float BestDot = -1.0f;

    for (AActor* OverlappedActor : OverlappingActors)
    {
       AMasterInteract* Interactable = Cast<AMasterInteract>(OverlappedActor);
       if (Interactable)
       {
          FVector PlayerCameraLocation = GetFirstPersonCameraComponent()->GetComponentLocation();
          FVector DirectionToActor = Interactable->GetActorLocation() - PlayerCameraLocation;
          DirectionToActor.Normalize();
          float DotProduct = FVector::DotProduct(GetFirstPersonCameraComponent()->GetForwardVector(), DirectionToActor);

          if (DotProduct > InteractionDotThreshold && DotProduct > BestDot)
          {
             BestDot = DotProduct;
             BestInteractable = Interactable;
          }
       }
    }

    // مدیریت ویجت نام آیتم (این بخش درست است)
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

    // بخش ۲: منطق Ghost Mesh (این بخش اصلاح شده و بدون تکرار است)
    // این منطق فقط زمانی اجرا می‌شود که آیتمی در دست داریم و در حالت گذاشتن هستیم
    if (HeldItem && bIsPlacementModeActive)
    {
       FVector TraceStart = GetFirstPersonCameraComponent()->GetComponentLocation();
       FVector TraceEnd = TraceStart + (GetFirstPersonCameraComponent()->GetForwardVector() * InteractionDistance);
       FHitResult HitResult;

       float SphereRadius = 15.0f;
       FCollisionShape Sphere = FCollisionShape::MakeSphere(SphereRadius);

       bool bHasHit = GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, ECC_Visibility, Sphere);
       
       DrawDebugSphere(GetWorld(), bHasHit ? HitResult.ImpactPoint : TraceEnd, SphereRadius, 12, FColor::Green, false, -1.0f);

       if (bHasHit)
       {
          AWorkTable* TargetTable = Cast<AWorkTable>(HitResult.GetActor());
          GhostMeshComponent->SetStaticMesh(HeldItem->GetItemMesh()->GetStaticMesh());
          GhostMeshComponent->SetMaterial(0, TargetTable ? GhostMaterial_Green : GhostMaterial_Red);
          GhostMeshComponent->SetWorldLocation(HitResult.Location);
          GhostMeshComponent->SetVisibility(true);
       }
       else
       {
          GhostMeshComponent->SetVisibility(false);
       }
    }
    else
    {
       // در هر حالت دیگری، مطمئن می‌شویم که روح مخفی است
       if (GhostMeshComponent->IsVisible())
       {
          GhostMeshComponent->SetVisibility(false);
       }
    }
}











// EvilChefCharacter.cpp

void AEvilChefCharacter::PlaceItem()
{
	if (!HeldItem) { return; }

	UE_LOG(LogTemp, Warning, TEXT("Placing item: %s"), *HeldItem->GetName());

	// آیتم را از بازیکن جدا کن
	HeldItem->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	
	// آن را در محل Ghost Mesh قرار بده
	HeldItem->SetActorLocation(GhostMeshComponent->GetComponentLocation());

	// برخورد (Collision) را دوباره فعال کن
	HeldItem->SetActorEnableCollision(true);

	// ریست کردن متغیرها
	HeldItem = nullptr;
	bIsPlacementModeActive = false;
	GhostMeshComponent->SetVisibility(false);
}