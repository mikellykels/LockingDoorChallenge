// Fill out your copyright notice in the Description page of Project Settings.


#include "LDKey.h"
#include "LDCharacter.h"
#include "InventoryComponent.h"
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

	RotationSpeed = 90.0f;
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
			FKeyType KeyTypeProperties = GetKeyTypeProperties();
			Widget->SetKeyDisplayName(KeyTypeProperties.KeyDisplayName, KeyTypeProperties.KeyColor);

			Widget->SetVisibility(ESlateVisibility::Hidden);
			Widget->SetPromptText(FText::FromString("Press 'E' to pickup"));

		}
	}
}

void ALDKey::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ALDKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Create a rotation in yaw.
	FRotator Rotation = FRotator(0, RotationSpeed * DeltaTime, 0);

	// Rotate the actor.
	AddActorLocalRotation(Rotation);
}

void ALDKey::OnInteract_Implementation(AActor* Caller)
{
	if (ALDCharacter* Character = Cast<ALDCharacter>(Caller))
	{
		if (Character->InventoryComponent->PickupKey(this))
		{
			HidePrompt_Implementation();

			// This will hide the actor in the game when interacted with
			this->SetActorHiddenInGame(true);

			// Disable the actor's collision
			this->SetActorEnableCollision(false);
			Character->PickupKey(this);
		}
	}
}

void ALDKey::ShowPrompt_Implementation()
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

void ALDKey::HidePrompt_Implementation()
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

TArray<FName> ALDKey::GetInventory_Implementation() const
{
	return TArray<FName>();
}

FKeyType ALDKey::GetKeyTypeProperties() const
{
	if (KeyTypes)
	{
		return *KeyTypes->FindRow<FKeyType>(KeyType, "");
	}

	return FKeyType();
}

