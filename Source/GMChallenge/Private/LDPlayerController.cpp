// Fill out your copyright notice in the Description page of Project Settings.


#include "LDPlayerController.h"
#include "LDCharacter.h"
#include "LDKey.h"
#include "LDDoor.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Interactable.h"
#include "DrawDebugHelpers.h"

ALDPlayerController::ALDPlayerController()
{
}

void ALDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

}

void ALDPlayerController::Tick(float DeltaTime)
{
  if (GetCharacter())
  {

    FHitResult HitResult = PerformLineTrace();

    if (HitResult.GetActor())
    {
      // Check if the hit actor implements the IInteractable interface.
      if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
      {
        HandleKeyInteraction(HitResult);
        HandleDoorInteraction(HitResult);
      }
    }
    else
    {
      // If there's no hit, hide the interaction prompt on the last interacted key, if any.
      HideKeyInteractionPrompt();
      HideDoorInteractionPrompt();
    }
  }
}

FHitResult ALDPlayerController::PerformLineTrace()
{
  // Define the start and end points of the line trace.
  FVector Start = GetCharacter()->GetActorLocation();
  FVector ForwardVector = GetCharacter()->GetActorForwardVector();
  FVector End = Start + ForwardVector * InteractionDistance;

  FHitResult HitResult;
  GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);
  return HitResult;
}

void ALDPlayerController::HandleKeyInteraction(const FHitResult& HitResult)
{
  // Cast the hit actor to ALDKey to call the ShowPrompt function.
  ALDKey* KeyActor = Cast<ALDKey>(HitResult.GetActor());
  if (LastInteractedKey != KeyActor)
  {
    if (LastInteractedKey != nullptr && LastInteractedKey->IsValidLowLevel())
    {
      //LastInteractedKey->HidePrompt_Implementation();
      IInteractable::Execute_HidePrompt(LastInteractedKey);
    }
    LastInteractedKey = KeyActor;
  }
  if (LastInteractedKey && LastInteractedKey->IsValidLowLevel())
  {
    //LastInteractedKey->ShowPrompt_Implementation();
    IInteractable::Execute_ShowPrompt(LastInteractedKey);
  }

  // If the interact button is pressed, call the OnInteract function.
  if (bInteractInput)
  {
    IInteractable::Execute_OnInteract(HitResult.GetActor(), GetCharacter());
  }
}

void ALDPlayerController::HandleDoorInteraction(const FHitResult& HitResult)
{
  // Check if the hit result is a door.
  ALDDoor* Door = Cast<ALDDoor>(HitResult.GetActor());
  // If the door is valid and it's not the same as the current door.
  if (Door && Door != CurrentDoor)
  {
    // If there is a current door, hide its interaction widget.
    if (CurrentDoor)
    {
      //CurrentDoor->HidePrompt_Implementation();
      IInteractable::Execute_HidePrompt(CurrentDoor);
    }

    // Set the current door to the new door and show its interaction widget.
    CurrentDoor = Door;
    //CurrentDoor->ShowPrompt_Implementation();
    IInteractable::Execute_ShowPrompt(CurrentDoor);
  }
}

void ALDPlayerController::HideKeyInteractionPrompt()
{
  if (LastInteractedKey != nullptr && LastInteractedKey->IsValidLowLevel())
  {
    //LastInteractedKey->HidePrompt_Implementation();
    IInteractable::Execute_HidePrompt(LastInteractedKey);
    LastInteractedKey = nullptr;
  }
}

void ALDPlayerController::HideDoorInteractionPrompt()
{
  if (CurrentDoor)
  {
    // If the player is not looking at a door but there is a current door, hide its interaction widget.
    //CurrentDoor->HidePrompt_Implementation();
    IInteractable::Execute_HidePrompt(CurrentDoor);
    CurrentDoor = nullptr;
  }
}

void ALDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

  if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
  {
    // Moving
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALDPlayerController::Move);

    // Looking
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALDPlayerController::Look);

    // Sprint
    EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ALDPlayerController::SprintTriggered);
    EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ALDPlayerController::SprintReleased);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ALDPlayerController::OnInteractTriggered);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ALDPlayerController::OnInteractReleased);
  }
}

void ALDPlayerController::Move(const FInputActionValue& Value)
{
	ALDCharacter* LDCharacter = Cast<ALDCharacter>(GetPawn());

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (LDCharacter != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = LDCharacter->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		LDCharacter->AddMovementInput(ForwardDirection, MovementVector.Y);
		LDCharacter->AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ALDPlayerController::Look(const FInputActionValue& Value)
{
  ALDCharacter* LDCharacter = Cast<ALDCharacter>(GetPawn());

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (LDCharacter != nullptr)
	{
		// add yaw and pitch input to controller
		LDCharacter->AddControllerYawInput(LookAxisVector.X);
		LDCharacter->AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALDPlayerController::SprintTriggered()
{
  UE_LOG(LogTemp, Warning, TEXT("Sprint triggered"));
  ALDCharacter* LDCharacter = Cast<ALDCharacter>(GetPawn());

  if (LDCharacter != nullptr)
  {
    LDCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
  }
}

void ALDPlayerController::SprintReleased()
{
  UE_LOG(LogTemp, Warning, TEXT("Sprint released"));
  ALDCharacter* LDCharacter = Cast<ALDCharacter>(GetPawn());

  if (LDCharacter != nullptr)
  {
    LDCharacter->GetCharacterMovement()->MaxWalkSpeed = DefaultSpeed;
  }
}

void ALDPlayerController::OnInteractTriggered(const FInputActionValue& Value)
{
	bInteractInput = true;
}

void ALDPlayerController::OnInteractReleased(const FInputActionValue& Value)
{
	bInteractInput = false;
}

