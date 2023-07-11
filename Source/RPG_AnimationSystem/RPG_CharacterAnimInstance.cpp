// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_CharacterAnimInstance.h"
#include "RPG_Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void URPG_CharacterAnimInstance::NativeInitializeAnimation()
{
	CharacterOwner = Cast<ARPG_Character>(TryGetPawnOwner());
}

void URPG_CharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (CharacterOwner == nullptr) return;
	
	CharMovementComp = CharacterOwner->GetCharacterMovement();

	if (CharMovementComp == nullptr) return;

	CharacterVelocity = CharMovementComp->Velocity;
	Speed = CharMovementComp->Velocity.Size2D();

	if (CharMovementComp->GetCurrentAcceleration().Size2D() != 0 && Speed > 5.f)
	{
		bShouldMove = true;
	}
	bShouldMove = false;

	bSprinting = CharacterOwner->bIsSprinting;

	bIsFalling = CharMovementComp->IsFalling();
	
}
