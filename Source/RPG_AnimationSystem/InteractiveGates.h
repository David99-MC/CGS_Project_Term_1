// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "InteractiveGates.generated.h"

/**
 * 
 */
UCLASS()
class RPG_ANIMATIONSYSTEM_API AInteractiveGates : public ABasePickup
{
	GENERATED_BODY()

public:
	AInteractiveGates();

	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteInteraction() override;

	UFUNCTION()
	void OpenDoor(float CurveValue);

	// Check if the player is facing forward so we can open/close door from 2 side
	void SetIsPlayerFacingForward();

protected:
	virtual void BeginPlay() override;

public:
	bool bIsDoorOpened = false;

	bool bIsPlayerFacingForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* RightDoor;

	float DoorOpenAngle = -70.f;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DoorCurveFloat;
	
};
