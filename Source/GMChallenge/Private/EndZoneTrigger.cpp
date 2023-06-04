// Fill out your copyright notice in the Description page of Project Settings.


#include "EndZoneTrigger.h"
#include "EndLevelWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEndZoneTrigger::AEndZoneTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	RootComponent = TriggerVolume;
}

// Called when the game starts or when spawned
void AEndZoneTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &AEndZoneTrigger::OnOverlapBegin);
}

// Called every frame
void AEndZoneTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEndZoneTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn() && EndLevelWidgetClass)
	{
		// cast to player character
		ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);

		// Stop movement
		UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();
		if (CharacterMovement)
		{
			CharacterMovement->MovementMode = MOVE_None;
		}

		// Create our end level widget and add it to the viewport
		UUserWidget* Widget = CreateWidget<UEndLevelWidget>(GetWorld(), EndLevelWidgetClass);
		if (Widget)
		{
			Widget->AddToViewport();

			// Get the player controller
			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

			// Set input mode to UI only
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(Widget->TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);

			// Show mouse cursor
			PlayerController->bShowMouseCursor = true;
		}
	}
}

