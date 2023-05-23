// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "InteractionWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class GMCHALLENGE_API UInteractionWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:
	UInteractionWidgetComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
