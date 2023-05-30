// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Engine/DataTable.h"
#include "LDGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class GMCHALLENGE_API ALDGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Key")
	UDataTable* KeyTypes;
};
