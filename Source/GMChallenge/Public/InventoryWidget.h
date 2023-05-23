// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;

/**
 * 
 */
UCLASS()
class GMCHALLENGE_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UVerticalBox* KeyList;

	UFUNCTION(BlueprintCallable, Category = UI)
	void SetInventoryComponent(UInventoryComponent* InventoryComponent);

	UFUNCTION()
	void UpdateInventory();

private:

	UPROPERTY()
	UInventoryComponent* InventoryComponent;
};
