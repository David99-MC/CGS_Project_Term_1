// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "RPG_CharacterMovementComponent.generated.h"

UENUM(BlueprintType)
namespace ECustomMovementMode
{
	enum Type
	{
		MOVE_Climb UMETA(DisplayName = "Climbing")

	};
}

UCLASS()
class RPG_ANIMATIONSYSTEM_API URPG_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void DisplayMovementMode();

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual float GetMaxSpeed() const override;

	virtual float GetMaxAcceleration() const override;

public:
	TArray<FHitResult> DoCapsuleTraceMultiForObjects(const FVector& Start, const FVector& End, bool bShowDebugShape = false, bool bPersistentLine = false);

	FHitResult DoLineTraceSingleForObject(const FVector& Start, const FVector& End, bool bShowDebugShape = false, bool bPersistentLine = false);

	// Trace for climbable surfaces, returns true if there's any, false otherwise
	bool TraceClimbableSurfaces();

	// Trace for ledges
	bool TraceFromEyeHeight(float TraceDistance, float TraceStartOffset = 0.f);

	// Checking if we're using our custom MovementMode and the CustomMovementMode is equal to our custom enum type (Climb Mode)
	bool IsClimbing() const;

	void ToggleClimbing(bool bEnableClimbing);

	bool CanStartClimbing();

	void StartClimbing();

	void StopClimbing();

	void PhysClimb(float deltaTime, int32 Iterations);

	void ProcessClimbableSurfaces();
	
	/* 
		We need FQuat instead of FRotator because SafeMoveUpdatedComponent()
		requires an FQuat for its parameter
	*/
	FQuat GetClimbRotation(float DeltaTime); 

	FORCEINLINE FVector GetClimbableSurfaceNormal() { return CurrentClimbableSurfaceNormal; }

	void SnapMovementToClimbableSurfaces(float DeltaTime);

public:
	// Configurable Climb variables
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character Movement: Climbing")
	TArray< TEnumAsByte<EObjectTypeQuery> > ClimbableSurfaceTraceTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character Movement: Climbing")
	float ClimbCapsuleTraceRadius = 50.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character Movement: Climbing")
	float ClimbCapsuleTraceHalfHeight = 72.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character Movement: Climbing")
	float ClimbCapsuleTraceLength = 35.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character Movement: Climbing")
	float MaxBreakClimbDeceleration = 400.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character Movement: Climbing")
	float MaxClimbSpeed = 100.f; // Use when snapping movement to the surface

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = "Character Movement: Climbing")
	float MaxClimbAcceleration = 300.f;

private:
	TArray<FHitResult> ClimbableSurfacesTraceHitResults;

	FHitResult EyeHeightTraceHitResult;

	FVector CurrentClimbableSurfaceLocation;

	FVector CurrentClimbableSurfaceNormal;
	
};
