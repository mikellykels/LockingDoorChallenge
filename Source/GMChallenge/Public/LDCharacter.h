// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "LDCharacter.generated.h"

class ALDKey;
class UInventoryComponent;
class UInventoryWidget;

UCLASS()
class ALDCharacter : public ACharacter, public IInteractable
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

public:
	ALDCharacter();

	virtual void OnInteract_Implementation(AActor* Caller) override;
	virtual void ShowPrompt_Implementation() override;
	virtual void HidePrompt_Implementation() override;
	virtual TArray<FName> GetInventory_Implementation() const override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { 
		return CameraBoom; 
	}
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { 
		return FollowCamera; 
	}

	/** Inventory component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* InventoryComponent;

	UInventoryComponent* GetInventoryComponent() const { 
		return InventoryComponent; 
	}

	void PickupKey(ALDKey* Key);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;

	UInventoryWidget* InventoryWidget;

	UPROPERTY()
	TArray<FName> Inventory;
};