// Fill out your copyright notice in the Description page of Project Settings.


#include "EndLevelWidget.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UEndLevelWidget::RestartLevel()
{
	UGameplayStatics::OpenLevel(this, *GetWorld()->GetName(), false);

	ACharacter* PlayerCharacter = Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());

	// Enable character movement
	PlayerCharacter->EnableInput(GetWorld()->GetFirstPlayerController());

	// Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	// Set input mode to game only
	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	// Hide mouse cursor
	PlayerController->bShowMouseCursor = false;
}

void UEndLevelWidget::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
