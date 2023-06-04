// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "LDKey.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UInventoryComponent::CanOpenDoor(const FDoorType& DoorType) const
{
	for (FName KeyType : DoorType.RequiredKeys)
	{
		if (!Inventory.Contains(KeyType) || Inventory[KeyType] <= 0)
		{
			// If any required key is not in the inventory, or if the count of the key is 0,
			// then return false.
			return false;
		}
	}
	// If we have made it past the for loop, then all required keys are present in the inventory.
	return true;
}

void UInventoryComponent::UseKey(FName KeyType)
{
	if (Inventory.Contains(KeyType) && Inventory[KeyType] > 0)
	{
		Inventory[KeyType]--;
		OnInventoryChanged.Broadcast();
	}
}

bool UInventoryComponent::PickupKey(ALDKey* Key)
{
	if (Key)
	{
		FName KeyType = Key->GetKeyType();

		if (Inventory.Contains(KeyType))
		{
			Inventory[KeyType]++;
		}
		else
		{
			Inventory.Add(KeyType, 1);
		}
		OnInventoryChanged.Broadcast();
		return true;
	}
	return false;
}

