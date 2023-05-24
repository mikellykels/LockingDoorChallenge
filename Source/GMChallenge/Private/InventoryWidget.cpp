// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "InventoryComponent.h"
#include "LDGameStateBase.h"
#include "KeyType.h"
#include "Components/TextBlock.h"
#include "Engine/DataTable.h"
#include "Styling/SlateColor.h"

void UInventoryWidget::SetInventoryComponent(UInventoryComponent* NewInventoryComponent)
{
  // If the InventoryComponent already exists, unbind the delegate from the existing component
  if (IsValid(InventoryComponent))
  {
    InventoryComponent->OnInventoryChanged.RemoveAll(this);
  }

  // Assign the new inventory component
  InventoryComponent = NewInventoryComponent;

  // Bind the UpdateInventory function to the OnInventoryChanged delegate
  InventoryComponent->OnInventoryChanged.AddDynamic(this, &UInventoryWidget::UpdateInventory);

  // Update the inventory display
  UpdateInventory();
}

void UInventoryWidget::UpdateInventory()
{
  // Make sure KeyList is not null before trying to clear its children
  if (IsValid(KeyList))
  {
    KeyList->ClearChildren();
  }

  if (IsValid(InventoryComponent))
  {
    TMap<FName, int32> KeysInventory = InventoryComponent->GetKeysInventory();

    for (const TPair<FName, int32>& KeyPair : KeysInventory)
    {
      FName KeyType = KeyPair.Key;
      int32 KeyCount = KeyPair.Value;

      // Skip this key if there are none left in the inventory
      if (KeyCount <= 0)
      {
        continue;
      }

      UDataTable* KeyTypes = GetWorld()->GetGameState<ALDGameStateBase>()->KeyTypes;

      static const FString ContextString(TEXT("KeyTypes context"));
      FKeyType* KeyTypeProperties = KeyTypes->FindRow<FKeyType>(KeyType, ContextString);

      if (KeyTypeProperties)
      {
        UTextBlock* KeyTextBlock = NewObject<UTextBlock>(this);
        KeyTextBlock->SetText(FText::Format(FText::FromString("{0} ({1})"), KeyTypeProperties->KeyDisplayName, KeyCount));

        FColor Color = KeyTypeProperties->KeyColor;
        FLinearColor LinearColor = FLinearColor(Color);
        LinearColor.A = 1.0f;

        FSlateColor KeyColor(LinearColor);
        KeyTextBlock->SetColorAndOpacity(KeyColor);

        KeyList->AddChild(KeyTextBlock);
      }
    }
  }
}
