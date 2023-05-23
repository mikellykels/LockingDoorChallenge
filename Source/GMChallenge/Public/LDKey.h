// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "KeyType.h"
#include "Components/WidgetComponent.h"
#include "LDKey.generated.h"

class UStaticMeshComponent;
class UInteractionWidgetComponent;
class UDataTable;

UCLASS()
class GMCHALLENGE_API ALDKey : public AActor, public IInteractable
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ALDKey();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName KeyType;

	UFUNCTION(BlueprintCallable, Category = "Key")
	FKeyType GetKeyTypeProperties() const;

	// A reference to the DataTable that contains key type properties.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Key")
	UDataTable* KeyTypes;

	UFUNCTION(BlueprintCallable, Category = "Key")
	FName GetKeyType() const { return KeyType; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Prompt)
	UInteractionWidgetComponent* InteractionWidgetComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnInteract_Implementation(AActor* Caller) override;

	void ShowPrompt();
	void HidePrompt();
};
