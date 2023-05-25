// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DoorInteractionWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class GMCHALLENGE_API UDoorInteractionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PromptText;

	UFUNCTION(BlueprintImplementableEvent)
	void SetPromptTextColor(FColor Color);

	void SetPromptText(const FText& Text);
};
