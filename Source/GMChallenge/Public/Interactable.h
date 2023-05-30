// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GMCHALLENGE_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void OnInteract(AActor* Caller);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void ShowPrompt();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	void HidePrompt();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	TArray<FName> GetInventory() const;

};
