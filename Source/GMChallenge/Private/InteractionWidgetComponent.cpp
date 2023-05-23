// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionWidgetComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

UInteractionWidgetComponent::UInteractionWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true; // Set to true to enable TickComponent to be called
}

void UInteractionWidgetComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Make widget face player
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
    ACharacter* PlayerCharacter = PC->GetCharacter();
    if (PlayerCharacter)
    {
      UCameraComponent* CameraComponent = PlayerCharacter->FindComponentByClass<UCameraComponent>();
      if (CameraComponent)
      {
        FRotator NewRotation = CameraComponent->GetComponentRotation();
        NewRotation.Pitch = 0.f; // keep the widget horizontal
        NewRotation.Yaw += 180.f; // flip the widget to face the camera
        SetWorldRotation(NewRotation);
      }
    }
	}
}