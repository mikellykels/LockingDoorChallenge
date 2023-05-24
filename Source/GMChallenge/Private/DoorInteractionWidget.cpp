// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorInteractionWidget.h"
#include "Components/TextBlock.h"

void UDoorInteractionWidget::SetPromptText(const FText& Text)
{
	if (PromptText)
	{
		PromptText->SetText(Text);
	}
}
