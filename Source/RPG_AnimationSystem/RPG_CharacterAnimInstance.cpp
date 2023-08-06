// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_CharacterAnimInstance.h"
#include "RPG_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void URPG_CharacterAnimInstance::NativeInitializeAnimation()
{
	CharacterOwner = Cast<ARPG_Character>(GetOwningActor());
}

void URPG_CharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (CharacterOwner == nullptr) return;

	CharMovementComp = CharacterOwner->GetCharacterMovement();

	GroundSpeed = CharMovementComp->Velocity.Size2D();

	bSprinting = CharacterOwner->bIsSprinting;

	bIsFalling = CharMovementComp->IsFalling();

	bIsCrouched = CharacterOwner->bIsCrouched;

	bCanTurnInPlace = CharacterOwner->bCanTurn;
	
}
