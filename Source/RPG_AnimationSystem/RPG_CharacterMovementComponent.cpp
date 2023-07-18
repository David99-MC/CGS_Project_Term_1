// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_CharacterMovementComponent.h"
#include "RPG_Character.h"
#include "Engine/Engine.h"

void URPG_CharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/*TraceClimbableSurfaces();
	TraceFromEyeHeight(100.f);*/
}

bool URPG_CharacterMovementComponent::TraceClimbableSurfaces()
{
	FVector StartOffset = UpdatedComponent->GetForwardVector() * 50.f;

	FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	FVector End = Start + UpdatedComponent->GetForwardVector();

	ClimbableSurfacesTraceHitResults = DoCapsuleTraceMultiForObjects(Start, End, true, true);

	return !ClimbableSurfacesTraceHitResults.IsEmpty();
}

TArray<FHitResult> URPG_CharacterMovementComponent::DoCapsuleTraceMultiForObjects(const FVector& Start, const FVector& End, bool bShowDebugShape, bool bPersistentLine)
{
	EDrawDebugTrace::Type bShowDebug = EDrawDebugTrace::None;
	if (bShowDebugShape)
	{
		bShowDebug = EDrawDebugTrace::ForOneFrame;
		if (bPersistentLine)
		{
			bShowDebug = EDrawDebugTrace::Persistent;
		}
	}

	TArray<FHitResult> OutHitResults;

	UKismetSystemLibrary::CapsuleTraceMultiForObjects(
		this,
		Start,
		End,
		ClimbCapsuleTraceRadius,
		ClimbCapsuleTraceHalfHeight,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		bShowDebug,
		OutHitResults,
		false
	);

	return OutHitResults;
}

bool URPG_CharacterMovementComponent::TraceFromEyeHeight(float TraceDistance, float TraceStartOffset)
{
	FVector ComponentLocation = UpdatedComponent->GetComponentLocation();

	FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + TraceStartOffset);
	FVector Start = ComponentLocation + EyeHeightOffset;
	FVector End = Start + (UpdatedComponent->GetForwardVector() * TraceDistance);

	EyeHeightTraceHitResult = DoLineTraceSingleForObject(Start, End, true, true);

	return EyeHeightTraceHitResult.bBlockingHit;
}

FHitResult URPG_CharacterMovementComponent::DoLineTraceSingleForObject(const FVector& Start, const FVector& End, bool bShowDebugShape, bool bPersistentLine)
{
	EDrawDebugTrace::Type bShowDebug = EDrawDebugTrace::None;
	if (bShowDebugShape)
	{
		bShowDebug = EDrawDebugTrace::ForOneFrame;
		if (bPersistentLine)
		{
			bShowDebug = EDrawDebugTrace::Persistent;
		}
	}

	FHitResult OutHitResult;

	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ClimbableSurfaceTraceTypes,
		false,
		TArray<AActor*>(),
		bShowDebug,
		OutHitResult,
		false,
		FLinearColor::Blue
	);

	return OutHitResult;
}

bool URPG_CharacterMovementComponent::IsClimbing()
{
	return MovementMode == MOVE_Custom && CustomMovementMode == ECustomMovementMode::MOVE_Climb;
}

void URPG_CharacterMovementComponent::ToggleClimbing(bool bEnableClimbing)
{
	if (bEnableClimbing)
	{
		if (CanStartClimbing())
		{
			// Start Climbing
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Green, TEXT("Can climb now"));
			StartClimbing();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, TEXT("Can NOT climb now"));
		}
	}
	else
	{
		// Stop Climbing
		StopClimbing();
	}
}

void URPG_CharacterMovementComponent::StartClimbing()
{
	SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::MOVE_Climb);
}

void URPG_CharacterMovementComponent::StopClimbing()
{
	SetMovementMode(EMovementMode::MOVE_Falling);
}

bool URPG_CharacterMovementComponent::CanStartClimbing()
{
	return !IsFalling() && TraceClimbableSurfaces() && TraceFromEyeHeight(100.f);
}
