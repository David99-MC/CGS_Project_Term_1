// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPG_CharacterAnimInstance.generated.h"

class ARPG_Character;
class URPG_CharacterMovementComponent;

UCLASS()
class RPG_ANIMATIONSYSTEM_API URPG_CharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	ARPG_Character* CharacterOwner;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	URPG_CharacterMovementComponent* CharacterMovementComp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bSprinting;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bCanTurnInPlace;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsClimbing;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector ClimbVelocity;
};
