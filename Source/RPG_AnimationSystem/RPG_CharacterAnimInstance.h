// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPG_CharacterAnimInstance.generated.h"

class ARPG_Character;
class UCharacterMovementComponent;

UCLASS()
class RPG_ANIMATIONSYSTEM_API URPG_CharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaTime) override;

private:
	ARPG_Character* CharacterOwner;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UCharacterMovementComponent* CharMovementComp;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bSprinting;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsFalling;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool bIsCrouched;


};
