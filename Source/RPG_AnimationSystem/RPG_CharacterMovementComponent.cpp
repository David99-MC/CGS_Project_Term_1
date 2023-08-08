// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_CharacterMovementComponent.h"
#include "RPG_Character.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"

void URPG_CharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//DisplayMovementMode();
}

void URPG_CharacterMovementComponent::DisplayMovementMode()
{
	FText MovementModeName;
	UEnum::GetDisplayValueAsText(MovementMode, MovementModeName);

	TEnumAsByte<ECustomMovementMode::Type> Byte = (ECustomMovementMode::Type)CustomMovementMode;
	FText CustomMovemementModeName;
	UEnum::GetDisplayValueAsText(Byte, CustomMovemementModeName);

	FString CheckMovementModeName = FString::Printf(TEXT("MovementMode name: %s, CustomMovementMode name: %s"), *MovementModeName.ToString(), *CustomMovemementModeName.ToString());
	GEngine->AddOnScreenDebugMessage(5, 2.f, FColor::Cyan, CheckMovementModeName);
}

void URPG_CharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	if (IsClimbing())
	{
		if (ARPG_Character* Player = Cast<ARPG_Character>(CharacterOwner))
		{
			Player->bCanTurn = false;
			bOrientRotationToMovement = false;
			bUseControllerDesiredRotation = false;
			Player->bUseControllerRotationYaw = false;
		}
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(44.f);
	}

	// Just Exit the Climbing State
	if (PreviousMovementMode == EMovementMode::MOVE_Custom && PreviousCustomMode == ECustomMovementMode::MOVE_Climb)
	{
		if (ARPG_Character* Player = Cast<ARPG_Character>(CharacterOwner))
		{
			Player->bCanTurn = true;
			bOrientRotationToMovement = true;
			bUseControllerDesiredRotation = true;
			Player->bUseControllerRotationYaw = true;
		}
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(88.f);

		StopMovementImmediately(); // cancel any excess movement from this state
	}

	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
}

void URPG_CharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	if (IsClimbing())
	{
		PhysClimb(deltaTime, Iterations);
	}

	Super::PhysCustom(deltaTime, Iterations);
}

void URPG_CharacterMovementComponent::PhysClimb(float deltaTime, int32 Iterations)
{
	if (deltaTime < MIN_TICK_TIME)
	{
		return;
	}

	/*Process all the climbable surfaces info*/
	TraceClimbableSurfaces();
	ProcessClimbableSurfaces();

	/*TODO: Check if we should stop climbing*/


	RestorePreAdditiveRootMotionVelocity();

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{	//Define the max climb speed and acceleration
		CalcVelocity(deltaTime, 0.f, true, MaxBreakClimbDeceleration);
	}

	ApplyRootMotionToVelocity(deltaTime);

	FVector OldLocation = UpdatedComponent->GetComponentLocation();
	const FVector Adjusted = Velocity * deltaTime;
	FHitResult Hit(1.f);

	//Handle climb rotation
	FQuat ClimbRotation = GetClimbRotation(deltaTime);
	SafeMoveUpdatedComponent(Adjusted, ClimbRotation, true, Hit);

	if (Hit.Time < 1.f)
	{
		//adjust and try again
		HandleImpact(Hit, deltaTime, Adjusted);
		SlideAlongSurface(Adjusted, (1.f - Hit.Time), Hit.Normal, Hit, true);
	}

	if (!HasAnimRootMotion() && !CurrentRootMotion.HasOverrideVelocity())
	{
		Velocity = (UpdatedComponent->GetComponentLocation() - OldLocation) / deltaTime;
	}

	/*Snap movement to climbable surfaces*/
	SnapMovementToClimbableSurfaces(deltaTime);
}

float URPG_CharacterMovementComponent::GetMaxSpeed() const
{
	if (!IsClimbing()) 
		return Super::GetMaxSpeed();

	return MaxClimbSpeed;
}

float URPG_CharacterMovementComponent::GetMaxAcceleration() const
{
	if (!IsClimbing())
		return Super::GetMaxAcceleration();

	return MaxClimbAcceleration;
}

void URPG_CharacterMovementComponent::ProcessClimbableSurfaces()
{
	CurrentClimbableSurfaceLocation = FVector::ZeroVector;
	CurrentClimbableSurfaceNormal = FVector::ZeroVector;

	if (ClimbableSurfacesTraceHitResults.IsEmpty()) return;

	for (const FHitResult& HitResult : ClimbableSurfacesTraceHitResults)
	{
		CurrentClimbableSurfaceLocation += HitResult.ImpactPoint;
		CurrentClimbableSurfaceNormal += HitResult.ImpactNormal;
	}

	CurrentClimbableSurfaceLocation /= ClimbableSurfacesTraceHitResults.Num();
	CurrentClimbableSurfaceNormal = CurrentClimbableSurfaceNormal.GetSafeNormal();
}

FQuat URPG_CharacterMovementComponent::GetClimbRotation(float DeltaTime)
{
	FQuat CurrentQuat = UpdatedComponent->GetComponentQuat();

	if (HasAnimRootMotion() || CurrentRootMotion.HasOverrideVelocity())
	{
		// Let the RootMotion Animation handles the physics
		return CurrentQuat;
	}

	// Inverse the CurrentClimbableSurfaceNormal since we want a rotation that face towards the surface
	FQuat TargetQuat = FRotationMatrix::MakeFromX(-CurrentClimbableSurfaceNormal).ToQuat();

	return FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, 5.f); // could be configurable

}

void URPG_CharacterMovementComponent::SnapMovementToClimbableSurfaces(float DeltaTime)
{
	FVector ComponentForward = UpdatedComponent->GetForwardVector();
	FVector ComponentLocation = UpdatedComponent->GetComponentLocation();

	FVector ProjectedCharacterToSurface =
		(CurrentClimbableSurfaceLocation - ComponentLocation).ProjectOnTo(ComponentForward);

	// Inverse the CurrentClimbableSurfaceNormal since we to snap to its oppsite direction, 
	// which points towards the surface
	FVector SnapVector = -CurrentClimbableSurfaceNormal * ProjectedCharacterToSurface.Length();

	UpdatedComponent->MoveComponent(
		SnapVector * DeltaTime * MaxClimbSpeed, 
		UpdatedComponent->GetComponentQuat(), 
		true);
}

bool URPG_CharacterMovementComponent::TraceClimbableSurfaces()
{
	FVector StartOffset = UpdatedComponent->GetForwardVector() * ClimbCapsuleTraceLength;

	FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	FVector End = Start + UpdatedComponent->GetForwardVector();

	ClimbableSurfacesTraceHitResults = DoCapsuleTraceMultiForObjects(Start, End);

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

	EyeHeightTraceHitResult = DoLineTraceSingleForObject(Start, End);

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

bool URPG_CharacterMovementComponent::IsClimbing() const
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
			GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Green, TEXT("Can climb now"));
			StartClimbing();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(3, 2.f, FColor::Red, TEXT("Can NOT climb now"));
		}
	}
	else
	{
		// Stop Climbing
		StopClimbing();
	}
}

bool URPG_CharacterMovementComponent::CanStartClimbing()
{
	return !IsFalling() && TraceClimbableSurfaces() && TraceFromEyeHeight(100.f);
}

void URPG_CharacterMovementComponent::StartClimbing()
{
	SetMovementMode(EMovementMode::MOVE_Custom, ECustomMovementMode::MOVE_Climb);
}

void URPG_CharacterMovementComponent::StopClimbing()
{
	SetMovementMode(EMovementMode::MOVE_Falling);
}
