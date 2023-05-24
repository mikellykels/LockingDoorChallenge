// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Components/TimelineComponent.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "LDDoor.generated.h"

class UStaticMeshComponent;
class UDoorInteractionWidget;
class UWidgetComponent;
class UCurveFloat;

USTRUCT(BlueprintType)
struct FDoorType : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DoorName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> RequiredKeys;
};

UCLASS()
class GMCHALLENGE_API ALDDoor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALDDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interactable interface implementation
	virtual void OnInteract_Implementation(AActor* Caller) override;
	virtual void ShowPrompt_Implementation() override;
	virtual void HidePrompt_Implementation() override;
	virtual TArray<FName> GetInventory_Implementation() const override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* DoorFrameMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* DoorMeshComponent;

	// Door interaction widget component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* DoorInteractionWidgetComponent;

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UDoorInteractionWidget> DoorInteractionWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
	FName DoorType;


	UFUNCTION(BlueprintCallable, Category = Door)
	FDoorType GetDoorTypeProperties() const;

	// A reference to the DataTable that contains key type properties.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Door)
	UDataTable* DoorTypes;

	UPROPERTY(EditAnywhere, Category = Timeline)
	UCurveFloat* DoorOpenFloatCurve;

	FOnTimelineFloat InterpFunction{};
	FOnTimelineEvent TimelineFinished{};


	UPROPERTY()
	UTimelineComponent* DoorTimeline;

	UPROPERTY()
	float DoorOpenAngle;

	UPROPERTY()
	float DoorCloseAngle;

	UFUNCTION()
	void OnTimelineFloat(float Value);

private:
	void OpenDoor();
	bool bIsDoorOpen = false;
};
