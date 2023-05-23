// Fill out your copyright notice in the Description page of Project Settings.


#include "LDPlayerController.h"
#include "LDCharacter.h"
#include "LDKey.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
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
    // Define the start and end points of the line trace.
    FVector Start = GetCharacter()->GetActorLocation();
    FVector ForwardVector = GetCharacter()->GetActorForwardVector();
    FVector End = Start + ForwardVector * 500.f; // 500 is the reach of the interaction. Adjust as necessary.

    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility);

    if (HitResult.GetActor())
    {
      // Check if the hit actor implements the IInteractable interface.
      if (HitResult.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
      {
        // Cast the hit actor to ALDKey to call the ShowPrompt function.
        ALDKey* KeyActor = Cast<ALDKey>(HitResult.GetActor());
        if (LastInteractedKey != KeyActor)
        {
          if (LastInteractedKey != nullptr && LastInteractedKey->IsValidLowLevel())
          {
            LastInteractedKey->HidePrompt();
          }
          LastInteractedKey = KeyActor;
        }
        if (LastInteractedKey && LastInteractedKey->IsValidLowLevel())
        {
          LastInteractedKey->ShowPrompt();
        }

        // If the interact button is pressed, call the OnInteract function.
        if (bInteractInput)
        {
          IInteractable::Execute_OnInteract(HitResult.GetActor(), this);
        }
      }
    }
    else
    {
      // If there's no hit, hide the interaction prompt on the last interacted key, if any.
      if (LastInteractedKey != nullptr && LastInteractedKey->IsValidLowLevel())
      {
        LastInteractedKey->HidePrompt();
        LastInteractedKey = nullptr;
      }
    }
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

void ALDPlayerController::OnInteractTriggered(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
	bInteractInput = true;
}

void ALDPlayerController::OnInteractReleased(const FInputActionValue& Value)
{
	bInteractInput = false;
}

