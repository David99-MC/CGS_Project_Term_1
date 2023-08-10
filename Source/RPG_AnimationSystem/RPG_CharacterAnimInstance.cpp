// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_CharacterAnimInstance.h"
#include "RPG_Character.h"
#include "RPG_CharacterMovementComponent.h"

void URPG_CharacterAnimInstance::NativeInitializeAnimation()
{
	CharacterOwner = Cast<ARPG_Character>(GetOwningActor());
	if (CharacterOwner)
	{
		CharacterMovementComp = CharacterOwner->RPG_CharacterMovementComponent;
	}
}

void URPG_CharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (CharacterOwner == nullptr) return;

	GroundSpeed = CharacterMovementComp->Velocity.Size2D();

	bSprinting = CharacterOwner->bIsSprinting;

	bIsFalling = CharacterMovementComp->IsFalling();

	bIsCrouched = CharacterOwner->bIsCrouched;

	bCanTurnInPlace = CharacterOwner->bCanTurn;
	
}
