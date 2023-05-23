// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "InteractionPromptWidget.generated.h"

/**
 * 
 */
UCLASS()
class GMCHALLENGE_API UInteractionPromptWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* PromptText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* KeyDisplayNameTextBlock;
	
	void SetPromptText(const FText& Text);

	UFUNCTION(BlueprintCallable, Category = UI)
	void SetKeyDisplayName(FText DisplayName, FColor Color);
};
