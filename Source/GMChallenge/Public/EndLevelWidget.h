// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndLevelWidget.generated.h"

/**
 * 
 */
UCLASS()
class GMCHALLENGE_API UEndLevelWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "EndLevel")
	void RestartLevel();

	UFUNCTION(BlueprintCallable, Category = "EndLevel")
	void QuitGame();
	
};
