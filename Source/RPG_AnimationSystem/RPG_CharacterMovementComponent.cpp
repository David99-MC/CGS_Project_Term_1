// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_CharacterMovementComponent.h"
#include "RPG_Character.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"


void URPG_CharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningPlayerAnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();

	if (OwningPlayerAnimInstance)
	{
		OwningPlayerAnimInstance->OnMontageEnded.AddDynamic(this, &URPG_CharacterMovementComponent::OnClimbMontageEnded);
		OwningPlayerAnimInstance->OnMontageBlendingOut.AddDynamic(this, &URPG_CharacterMovementComponent::OnClimbMontageEnded);
	}
	
}

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
		CharacterOwner->GetCapsuleComponent()->SetCapsuleHalfHeight(60.f);
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

		FRotator DirtyRotation = UpdatedComponent->GetComponentRotation();
		FRotator CleanRotation = FRotator::ZeroRotator; // FRotator(0.f, DirtyRotation.Yaw, 0.f);
		UpdatedComponent->SetRelativeRotation(CleanRotation);

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

	/*Check if we should stop climbing*/
	if (ShouldStopClimbing() || HasReachedFloor())
	{
		StopClimbing();
	}

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

	if (HasReachedLedge())
	{
		PlayClimbMontage(ClimbToTopMontage);
	}
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

FVector URPG_CharacterMovementComponent::ConstrainAnimRootMotionVelocity(const FVector& RootMotionVelocity, const FVector& CurrentVelocity) const
{
	bool bIsRootMotionAnimPlaying =
		IsFalling() && OwningPlayerAnimInstance && OwningPlayerAnimInstance->IsAnyMontagePlaying();

	if (bIsRootMotionAnimPlaying)
	{
		return RootMotionVelocity;
	}
	else
	{
		return Super::ConstrainAnimRootMotionVelocity(RootMotionVelocity, CurrentVelocity);
	}
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

bool URPG_CharacterMovementComponent::ShouldStopClimbing()
{
	if (ClimbableSurfacesTraceHitResults.IsEmpty()) return true;

	float DotResult = FVector::DotProduct(CurrentClimbableSurfaceNormal, FVector::UpVector);
	float DeltaDegree = FMath::RadiansToDegrees(FMath::Acos(DotResult)); //Acos() returns radian
	//GEngine->AddOnScreenDebugMessage(3, 1.f, FColor::Emerald, FString::Printf(TEXT("DeltaDegree: %f"), DeltaDegree));

	return DeltaDegree < StopClimbingDegree;
}

bool URPG_CharacterMovementComponent::HasReachedFloor()
{
	FVector DownVector = -UpdatedComponent->GetUpVector();
	FVector StartOffset = DownVector * 50.f;
	FVector Start = UpdatedComponent->GetComponentLocation() + StartOffset;
	FVector End = Start + DownVector;

	TArray<FHitResult> PossibleFloorHits = DoCapsuleTraceMultiForObjects(Start, End);

	if (PossibleFloorHits.IsEmpty()) return false;

	for (FHitResult& PossibleFloorHit : PossibleFloorHits)
	{
		bool bReachedFloor = FVector::Parallel(PossibleFloorHit.ImpactNormal, DownVector) &&
			(GetUnrotatedVelocityVector().Z < -10); // Climbing down

		if (bReachedFloor)
		{
			return true;
		}
	}
	return false;
}

bool URPG_CharacterMovementComponent::HasReachedLedge()
{
	TraceFromEyeHeight(80.f, 20.f);

	if (EyeHeightTraceHitResult.bBlockingHit) // It's not ledge when there's still surface at eye height
		return false;

	FVector DownVector = -UpdatedComponent->GetUpVector();
	FVector Start = EyeHeightTraceHitResult.TraceEnd;
	FVector EndOffset = DownVector * 50.f;
	FVector End = Start + EndOffset;

	FHitResult LedgeHitResult = DoLineTraceSingleForObject(Start, End);

	return LedgeHitResult.bBlockingHit && GetUnrotatedVelocityVector().Z > 10.f;
}

bool URPG_CharacterMovementComponent::CanClimbDown()
{
	if (IsFalling()) 
		return false;

	FVector StartOnGround = UpdatedComponent->GetComponentLocation() + UpdatedComponent->GetForwardVector() * StartOnGroundDistance;
	FVector EndOnGround = StartOnGround - UpdatedComponent->GetUpVector() * 100.f;
	FHitResult GroundHitResult = DoLineTraceSingleForObject(StartOnGround, EndOnGround); 

	FVector StartClimbableSurface = StartOnGround + UpdatedComponent->GetForwardVector() * StartOnGroundOffset;
	FVector EndClimbableSurface = StartClimbableSurface - UpdatedComponent->GetUpVector() * 150.f;

	FHitResult ClimbDownSurface = DoLineTraceSingleForObject(StartClimbableSurface, EndClimbableSurface);

	return GroundHitResult.bBlockingHit && !ClimbDownSurface.bBlockingHit;
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

	return FMath::QInterpTo(CurrentQuat, TargetQuat, DeltaTime, 5.f); // InterpSpeed could be configurable

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
	FVector EyeHeightOffset = UpdatedComponent->GetUpVector() * (CharacterOwner->BaseEyeHeight + TraceStartOffset);
	FVector Start = UpdatedComponent->GetComponentLocation() + EyeHeightOffset;
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
			PlayClimbMontage(IdleToClimbMontage);
		}
		else if (CanClimbDown())
		{
			PlayClimbMontage(ClimbDownLedgeMontage);
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

void URPG_CharacterMovementComponent::PlayClimbMontage(UAnimMontage* MontageToPlay)
{
	if (OwningPlayerAnimInstance == nullptr || MontageToPlay == nullptr || OwningPlayerAnimInstance->IsAnyMontagePlaying())
		return;

	OwningPlayerAnimInstance->Montage_Play(MontageToPlay);
}

void URPG_CharacterMovementComponent::OnClimbMontageEnded(UAnimMontage* Montage, bool bInteruppted)
{
	if (Montage == IdleToClimbMontage || Montage == ClimbDownLedgeMontage)
	{
		StartClimbing();
		StopMovementImmediately(); // Disable any excess Velocity from RootMotion anim
	}

	if (Montage == ClimbToTopMontage)
	{
		SetMovementMode(EMovementMode::MOVE_Walking);
	}
	
}

FVector URPG_CharacterMovementComponent::GetUnrotatedVelocityVector()
{
	return UKismetMathLibrary::Quat_UnrotateVector(UpdatedComponent->GetComponentQuat(), Velocity);
}
