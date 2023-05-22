// Fill out your copyright notice in the Description page of Project Settings.


#include "LDPlayerController.h"
#include "LDCharacter.h"
#include "GMChallenge/GMChallengeCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"

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

void ALDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

  if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
  {
		AGMChallengeCharacter* GMChallengeCharacter = Cast<AGMChallengeCharacter>(GetPawn());

    // Moving
    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALDPlayerController::Move);

    // Looking
    EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALDPlayerController::Look);

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

