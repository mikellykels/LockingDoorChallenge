// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "KeyType.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FKeyType : public FTableRowBase
{
  GENERATED_BODY()

public:

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FName KeyName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FText KeyDisplayName;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  FColor KeyColor;

};
