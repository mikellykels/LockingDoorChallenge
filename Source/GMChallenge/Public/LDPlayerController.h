// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "LDPlayerController.generated.h"

class ALDKey;
class ALDDoor;

/**
 * 
 */
UCLASS()
class GMCHALLENGE_API ALDPlayerController : public APlayerController
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Sprint Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

public:
	ALDPlayerController();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for sprint input */
	void SprintTriggered();
	void SprintReleased();

	/** Called for interact input */
	void OnInteractTriggered(const FInputActionValue& Value);
	void OnInteractReleased(const FInputActionValue& Value);

	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	bool bInteractInput = false;
	float DefaultSpeed = 500.0f;
	float SprintSpeed = 800.0f;
	float InteractionDistance = 300.0f;

	ALDKey* LastInteractedKey = nullptr;
	ALDDoor* CurrentDoor = nullptr;

	FHitResult PerformLineTrace();
	void HandleKeyInteraction(const FHitResult& HitResult);
	void HandleDoorInteraction(const FHitResult& HitResult);
	void HideKeyInteractionPrompt();
	void HideDoorInteractionPrompt();
};
