// Fill out your copyright notice in the Description page of Project Settings.


#include "LDDoor.h"
#include "DoorInteractionWidget.h"
#include "LDCharacter.h"
#include "InventoryComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TimelineComponent.h"
#include "Curves/CurveFloat.h"

// Sets default values
ALDDoor::ALDDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//** Door frame mesh **//
	DoorFrameMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMeshComponent->SetupAttachment(RootComponent);

	//** Door mesh **//
	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMeshComponent->SetupAttachment(DoorFrameMeshComponent);

	//** Door interaction widget front **//
	DoorInteractionWidgetComponentFront = CreateDefaultSubobject<UWidgetComponent>(TEXT("DoorInteractionWidgetFront"));
	DoorInteractionWidgetComponentFront->SetupAttachment(DoorMeshComponent);
	DoorInteractionWidgetComponentFront->SetWidgetClass(DoorInteractionWidgetClass);

	//** Door interaction widget back **//
	DoorInteractionWidgetComponentBack = CreateDefaultSubobject<UWidgetComponent>(TEXT("DoorInteractionWidgetBack"));
	DoorInteractionWidgetComponentBack->SetupAttachment(DoorMeshComponent);
	DoorInteractionWidgetComponentBack->SetWidgetClass(DoorInteractionWidgetClass);

	// ** Door timeline ** //
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

	OpeningDirection = 1.0f;

	CurrentPromptText = FText::GetEmpty();
	CurrentPromptColor = FColor::White;

}

// Called when the game starts or when spawned
void ALDDoor::BeginPlay()
{
	Super::BeginPlay();

	// Set up the widgets
	SetupDoorInteractionWidget(DoorInteractionWidgetComponentFront);
	SetupDoorInteractionWidget(DoorInteractionWidgetComponentBack);

	if (DoorOpenFloatCurve)
	{
		InterpFunction.BindUFunction(this, FName("OnTimelineFloat"));

		DoorTimeline->AddInterpFloat(DoorOpenFloatCurve, InterpFunction);
		DoorTimeline->SetLooping(false);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("OpenCurve is not set in the editor"));
	}
}

// Called every frame
void ALDDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALDDoor::OnInteract_Implementation(AActor* Caller)
{
	if (ALDCharacter* Character = Cast<ALDCharacter>(Caller))
	{
		FDoorType DoorTypeProperties = GetDoorTypeProperties();
		if (bIsUnlocked || Character->InventoryComponent->CanOpenDoor(DoorTypeProperties))
		{
			// Open the door
			OpenDoor(Caller);

			// If the door was locked, remove key from inventory and set the door to unlocked
			if (!bIsUnlocked)
			{
				for (FName KeyType : DoorTypeProperties.RequiredKeys)
				{
					Character->InventoryComponent->UseKey(KeyType);
				}
				bIsUnlocked = true; // Set the door to unlocked
			}

			// If the player has the required keys, set the text color to white
			UpdateDoorInteractionWidget(DoorInteractionWidgetComponentFront, ESlateVisibility::Visible, FText::FromString("Press 'E' to interact"), FColor::White);
			UpdateDoorInteractionWidget(DoorInteractionWidgetComponentBack, ESlateVisibility::Visible, FText::FromString("Press 'E' to interact"), FColor::White);
		}
		else
		{
			// If the player does not have the required keys, update the message
			// Get the count of required keys
			int32 KeyCount = DoorTypeProperties.RequiredKeys.Num();

			// Create custom message
			FString Message;
			if (KeyCount == 1)
			{
				Message = FString::Printf(TEXT("Needs %d %s key"),
					KeyCount, *DoorTypeProperties.DoorName.ToString());
			}
			else // more than one key needed
			{
				FString KeyNames = "";
				for (int i = 0; i < KeyCount; i++)
				{
					KeyNames += DoorTypeProperties.RequiredKeys[i].ToString();
					if (i < KeyCount - 1)
					{
						KeyNames += ", ";
					}
				}
				Message = FString::Printf(TEXT("Needs %d keys: %s"), KeyCount, *KeyNames);
			}
			// Set the UI prompt to the custom message
			UpdateDoorInteractionWidget(DoorInteractionWidgetComponentFront, ESlateVisibility::Visible, FText::FromString(Message), FColor::Red);
			UpdateDoorInteractionWidget(DoorInteractionWidgetComponentBack, ESlateVisibility::Visible, FText::FromString(Message), FColor::Red);
		}
	}
}

void ALDDoor::ShowPrompt_Implementation()
{
	UpdateDoorInteractionWidget(DoorInteractionWidgetComponentFront, ESlateVisibility::Visible, CurrentPromptText, CurrentPromptColor);
	UpdateDoorInteractionWidget(DoorInteractionWidgetComponentBack, ESlateVisibility::Visible, CurrentPromptText, CurrentPromptColor);
}

void ALDDoor::HidePrompt_Implementation()
{
	UpdateDoorInteractionWidget(DoorInteractionWidgetComponentFront, ESlateVisibility::Hidden, CurrentPromptText, CurrentPromptColor);
	UpdateDoorInteractionWidget(DoorInteractionWidgetComponentBack, ESlateVisibility::Hidden, CurrentPromptText, CurrentPromptColor);
}

TArray<FName> ALDDoor::GetInventory_Implementation() const
{
	return TArray<FName>();
}

FDoorType ALDDoor::GetDoorTypeProperties() const
{
	if (DoorTypes)
	{
		const FDoorType* DoorTypeProperties = DoorTypes->FindRow<FDoorType>(DoorType, "");
		if (DoorTypeProperties)
		{
			return *DoorTypeProperties;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DoorTypes == nullptr"));
	}

	return FDoorType();
}

void ALDDoor::OnTimelineFloat(float Value)
{
	if (DoorMeshComponent)
	{
		// Calculate the new rotation value and set the actor rotation
		float DoorAngle = FMath::Lerp(DoorCloseAngle, DoorOpenAngle, Value);
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw = DoorAngle;
		DoorMeshComponent->SetRelativeRotation(NewRotation);
	}
}

void ALDDoor::OpenDoor(AActor* Interactor)
{
	UE_LOG(LogTemp, Warning, TEXT("OPEN DOOR"));
	if (bIsPlayingTimeline)
	{
		UE_LOG(LogTemp, Warning, TEXT("bIsPlayingTimeline"));
		return;
	}

	bIsPlayingTimeline = true;

	// Get the vector from the door to the player.
	FVector DoorToPlayer =  Interactor->GetActorLocation() - GetActorLocation();

	// Get the forward vector of the door.
	FVector DoorForward = GetActorForwardVector();

	// Use the dot product to determine if the player is in front of or behind the door.
	float DotProduct = FVector::DotProduct(DoorToPlayer, DoorForward);
	UE_LOG(LogTemp, Warning, TEXT("DotProduct: %f"), DotProduct);
	if (DotProduct > 0)
	{
		// Player is in front of the door.
		OpeningDirection = 1.0f;
	}
	else
	{
		// Player is behind the door.
		OpeningDirection = -1.0f;
	}

	// Set the rotation values
	DoorOpenAngle = OpeningDirection * 90.f;
	DoorCloseAngle = 0.0f;
	//DoorCloseAngle = -DoorOpenAngle;

	// Add the timeline events
	InterpFunction.BindUFunction(this, FName("OnTimelineFloat"));
	TimelineFinished.BindUFunction(this, FName("OnTimelineEnd"));

	DoorTimeline->AddInterpFloat(DoorOpenFloatCurve, InterpFunction, FName("Alpha"));
	DoorTimeline->SetTimelineFinishedFunc(TimelineFinished);

	if (bIsDoorOpen)
	{
		// Reverse the timeline if the door is already open
		DoorTimeline->ReverseFromEnd();
	}
	else
	{
		// Start the timeline if the door is closed
		DoorTimeline->PlayFromStart();
	}
	bIsDoorOpen = !bIsDoorOpen;
}

void ALDDoor::SetupDoorInteractionWidget(UWidgetComponent* WidgetComponent)
{
	if (WidgetComponent)
	{
		UDoorInteractionWidget* DoorWidget = Cast<UDoorInteractionWidget>(WidgetComponent->GetUserWidgetObject());
		if (DoorWidget)
		{
			DoorWidget->SetVisibility(ESlateVisibility::Hidden);
			DoorWidget->SetPromptText(FText::FromString("Press 'E' to interact"));
		}
	}
}

void ALDDoor::UpdateDoorInteractionWidget(UWidgetComponent* WidgetComponent, ESlateVisibility Visibility, FText Text, FColor Color)
{
	if (IsValid(WidgetComponent))
	{
		UDoorInteractionWidget* DoorWidget = Cast<UDoorInteractionWidget>(WidgetComponent->GetUserWidgetObject());
		if (IsValid(DoorWidget))
		{
			DoorWidget->SetVisibility(Visibility);
			if (!Text.IsEmpty()) // Only update the text if it's not empty
			{
				CurrentPromptText = Text;
				DoorWidget->SetPromptText(Text);
			}
			if (Color != FColor::Black)
			{
				CurrentPromptColor = Color;
				DoorWidget->SetPromptTextColor(Color);
			}
		}
	}
}

void ALDDoor::OnTimelineEnd()
{
	bIsPlayingTimeline = false;
}

