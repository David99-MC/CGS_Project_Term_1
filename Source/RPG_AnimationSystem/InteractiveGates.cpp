// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveGates.h"
#include "RPG_PlayerController.h"
#include "RPG_Character.h"

AInteractiveGates::AInteractiveGates()
{
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Door"));
	RightDoor->SetupAttachment(RootComponent);
}

void AInteractiveGates::BeginPlay()
{
	Super::BeginPlay();
	if (DoorCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AInteractiveGates::OpenDoor);

		Timeline.AddInterpFloat(DoorCurveFloat, TimelineProgress);
	}
}

void AInteractiveGates::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveGates::ExecuteInteraction()
{
	Super::ExecuteInteraction();
	//GEngine->AddOnScreenDebugMessage(2, 1.f, FColor::Green, FString::Printf(TEXT("IsFacingForward: %d"), IsPlayerFacingForward()));
	if (!PlayerController->bHasFinishedGetKeyObjective)
		return;

	if (!PlayerController->bHasFinishedOpenGatesObjective)
		PlayerController->InteractWithGatesObjective();

	// prevent openning the door while the player's back is facing the door
	if (!bIsDoorOpened) 
	{
		SetIsPlayerFacingForward();
		Timeline.Play();
	}
	else
	{
		Timeline.Reverse();
	}

	bIsDoorOpened = !bIsDoorOpened;
}

void AInteractiveGates::OpenDoor(float CurveValue)
{
	float ActualAngle = bIsPlayerFacingForward ? DoorOpenAngle : -DoorOpenAngle;

	FRotator LeftDoorRotation(0.f, ActualAngle * CurveValue, 0.f);

	FRotator RightDoorRotation(0.f, -ActualAngle * CurveValue, 0.f);

	// Set RELATIVE rotation so it rotates independently from its parent
	BaseMesh->SetRelativeRotation(LeftDoorRotation);
	RightDoor->SetRelativeRotation(RightDoorRotation);
}

void AInteractiveGates::SetIsPlayerFacingForward()
{
	FVector ThisFW = RootComponent->GetRightVector();
	FVector CurrentDirectionWithPlayer = OverlappingPlayer->GetActorLocation() - RootComponent->GetComponentLocation();

	bIsPlayerFacingForward = FVector::DotProduct(ThisFW, CurrentDirectionWithPlayer) > 0;
}


