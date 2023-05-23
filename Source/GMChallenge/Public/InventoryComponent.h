// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class ALDKey;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryChangedDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GMCHALLENGE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	bool PickupKey(ALDKey* Key);

	TMap<FName, int32> GetKeysInventory() const
	{
		return Inventory;
	}

	FInventoryChangedDelegate OnInventoryChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Map of key type to key count.
	TMap<FName, int32> Inventory;

};
