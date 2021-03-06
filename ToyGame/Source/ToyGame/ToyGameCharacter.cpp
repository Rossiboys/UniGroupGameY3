// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ToyGameCharacter.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "Containers/Array.h"

//////////////////////////////////////////////////////////////////////////
// AToyGameCharacter

AToyGameCharacter::AToyGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	isRewinding = false;
	RewindTime = 2;
}

void AToyGameCharacter::Tick(float DeltaTime)
{
	if (isRewinding == false)
	{
		StoreCharacterTransform();
	}
	else if (isRewinding == true)
	{
		RewindHazard();
	}

	LocArray.RemoveAt(121, DeltaTime);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AToyGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);


	PlayerInputComponent->BindAxis("MoveForward", this, &AToyGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AToyGameCharacter::MoveRight);


	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AToyGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AToyGameCharacter::LookUpAtRate);


	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AToyGameCharacter::OnResetVR);
}


void AToyGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AToyGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AToyGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AToyGameCharacter::MoveForward(float Value)
{
	// Prevents the player from moving if they are rewinding
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);


		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AToyGameCharacter::MoveRight(float Value)
{
	// Prevents the player from moving if they are rewinding
	if ( (Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AToyGameCharacter::AddBrick()
{
	totalBricks++;
}

///////////////////////////////////////////////////////////////
// This function is for storing the player's transform, eventually to be used in rewinding the player.
// Used in tick function
void AToyGameCharacter::StoreCharacterTransform()
{
	// Adds the actor transform to the array this will happen every tick (see line 84 .h)
	LocArray.Add(GetActorTransform());
}

// Activates the timer for the rewind
void AToyGameCharacter::ActivateRewind()
{
	if (isRewinding == false)
	{
		isRewinding = true;

		GetWorldTimerManager().SetTimer(ReverseTimerHandle, this, &AToyGameCharacter::RewindTimer, 1.0f, true);

		// Changes the tick interval so that the rewind is smoother and not instant
		PrimaryActorTick.TickInterval = 0.0166666666666667f;

	}
}

void AToyGameCharacter::RewindTimer()
{
	--RewindTime;
	if (RewindTime < 1)
	{
		GetWorldTimerManager().ClearTimer(ReverseTimerHandle);
		isRewinding = false;
		RewindTime = 2;

		// Changes the tick interval back to normal, so character transforms are stored correctly
		PrimaryActorTick.TickInterval = 0.0f;
	}
}

///////////////////////////////////////////////////////////////
// This function is for rewinding the player's position, giving the illusion that they're going back in time for a short period.
// Used in tick function

// TODO add posteffects to make it look cool

// END TODO
void AToyGameCharacter::RewindHazard()
{
	// Set's the actor's transform to be the last index in the array, aka the last location.
	SetActorTransform(LocArray.Last());

	// Removes the index at the end if it's greater than 0
	int32 i = LocArray.Num() - 1;
	if (i > 1)
	{
		LocArray.RemoveAt(i);
	}
}
