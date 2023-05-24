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

	//** Door frame mesh**//
	DoorFrameMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorFrameMesh"));
	DoorFrameMeshComponent->SetupAttachment(RootComponent);

	//** Door mesh **//
	DoorMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMeshComponent->SetupAttachment(DoorFrameMeshComponent);

	//** Door interaction widget **//
	DoorInteractionWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("DoorInteractionWidget"));
	DoorInteractionWidgetComponent->SetupAttachment(DoorMeshComponent);
	DoorInteractionWidgetComponent->SetWidgetClass(DoorInteractionWidgetClass);

	// ** Door timeline
	DoorTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DoorTimeline"));

}

// Called when the game starts or when spawned
void ALDDoor::BeginPlay()
{
	Super::BeginPlay();

	if (DoorInteractionWidgetComponent)
	{
		UDoorInteractionWidget* DoorWidget = Cast<UDoorInteractionWidget>(DoorInteractionWidgetComponent->GetUserWidgetObject());
		if (DoorWidget)
		{
			DoorWidget->SetVisibility(ESlateVisibility::Hidden);
			DoorWidget->SetPromptText(FText::FromString("Press 'E' to open"));

		}
	}

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
		if (Character->InventoryComponent->CanOpenDoor(DoorTypeProperties))
		{
			// Open the door
			OpenDoor();

			// Remove key from inventory
			for (FName KeyType : DoorTypeProperties.RequiredKeys)
			{
				Character->InventoryComponent->UseKey(KeyType);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Does not have required keys"));
			if (bIsDoorOpen)
			{
				OpenDoor();
			}
		}
	}
}

void ALDDoor::ShowPrompt_Implementation()
{
	// Make sure the widget component is valid before trying to hide it
	if (IsValid(DoorInteractionWidgetComponent))
	{
		UDoorInteractionWidget* DoorWidget = Cast<UDoorInteractionWidget>(DoorInteractionWidgetComponent->GetUserWidgetObject());
		if (IsValid(DoorWidget))
		{
			DoorWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void ALDDoor::HidePrompt_Implementation()
{
	// Make sure the widget component is valid before trying to show it
	if (IsValid(DoorInteractionWidgetComponent))
	{
		UDoorInteractionWidget* DoorWidget = Cast<UDoorInteractionWidget>(DoorInteractionWidgetComponent->GetUserWidgetObject());
		if (IsValid(DoorWidget))
		{
			DoorWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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
		FRotator NewRotation = DoorMeshComponent->GetRelativeRotation();
		NewRotation.Yaw = FMath::Lerp(0.0f, 90.0f, Value);
		DoorMeshComponent->SetRelativeRotation(NewRotation);
	}
}

void ALDDoor::OpenDoor()
{
	if (!bIsDoorOpen) // If the door is not open, open it.
	{
		UE_LOG(LogTemp, Warning, TEXT("Open door"));
		bIsDoorOpen = true; // toggle door state
		DoorTimeline->PlayFromStart();
	}
	else // If the door is open, close it.
	{
		UE_LOG(LogTemp, Warning, TEXT("Close door"));
		bIsDoorOpen = false;
		DoorTimeline->ReverseFromEnd();
	}
}

