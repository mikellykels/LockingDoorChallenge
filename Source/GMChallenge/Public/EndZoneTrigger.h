// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EndZoneTrigger.generated.h"

class UBoxComponent;
class UUserWidget;
class UEndLevelWidget;

UCLASS()
class GMCHALLENGE_API AEndZoneTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndZoneTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEndLevelWidget> EndLevelWidgetClass;

};
