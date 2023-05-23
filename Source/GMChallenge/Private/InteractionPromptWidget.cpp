// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionPromptWidget.h"

void UInteractionPromptWidget::SetPromptText(const FText& Text)
{
	if (PromptText)
	{
		UE_LOG(LogTemp, Warning, TEXT("Set prompt text widget"));
		PromptText->SetText(Text);
	}
}
