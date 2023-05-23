// Fill out your copyright notice in the Description page of Project Settings.


#include "LDKey.h"
#include "InteractionWidgetComponent.h"
#include "InteractionPromptWidget.h"

// Sets default values
ALDKey::ALDKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	InteractionWidgetComponent = CreateDefaultSubobject<UInteractionWidgetComponent>(TEXT("InteractionWidgetComponent"));
	InteractionWidgetComponent->SetupAttachment(RootComponent);
	InteractionWidgetComponent->SetWidgetClass(UInteractionPromptWidget::StaticClass());

}

// Called when the game starts or when spawned
void ALDKey::BeginPlay()
{
	Super::BeginPlay();
	
	if (InteractionWidgetComponent)
	{
		UInteractionPromptWidget* Widget = Cast<UInteractionPromptWidget>(InteractionWidgetComponent->GetUserWidgetObject());
		if (Widget)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
			Widget->SetPromptText(FText::FromString("Press 'E' to pickup"));
		}
	}
}

void ALDKey::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UE_LOG(LogTemp, Warning, TEXT("ALDKey instance ending play."));
}

// Called every frame
void ALDKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALDKey::OnInteract_Implementation(AActor* Caller)
{
	UE_LOG(LogTemp, Warning, TEXT("On Interact in Key"));
	HidePrompt();

	// This will hide the actor in the game when interacted with
	this->SetActorHiddenInGame(true);

	// Disable the actor's collision
	this->SetActorEnableCollision(false);
}

void ALDKey::ShowPrompt()
{
	// Make sure the widget component is valid before trying to hide it
	if (IsValid(InteractionWidgetComponent))
	{
		UInteractionPromptWidget* Widget = Cast<UInteractionPromptWidget>(InteractionWidgetComponent->GetUserWidgetObject());
		if (IsValid(Widget))
		{
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ALDKey::HidePrompt()
{
	// Make sure the widget component is valid before trying to show it
	if (IsValid(InteractionWidgetComponent))
	{
		UInteractionPromptWidget* Widget = Cast<UInteractionPromptWidget>(InteractionWidgetComponent->GetUserWidgetObject());
		if (IsValid(Widget))
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
