// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionPromptWidget.h"
#include "Styling/SlateColor.h"

void UInteractionPromptWidget::SetPromptText(const FText& Text)
{
	if (PromptText)
	{
		PromptText->SetText(Text);
	}
}

void UInteractionPromptWidget::SetKeyDisplayName(FText DisplayName, FColor Color)
{
	KeyDisplayNameTextBlock->SetText(DisplayName);

	FLinearColor LinearColor = FLinearColor(Color);
	LinearColor.A = 1.0f;

	FSlateColor KeyColor(LinearColor);
	KeyDisplayNameTextBlock->SetColorAndOpacity(KeyColor);
}
