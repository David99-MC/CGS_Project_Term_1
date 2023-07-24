// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPG_AnimationSystemGameModeBase.h"
#include "RPG_Character.h"

void ARPG_AnimationSystemGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &ARPG_AnimationSystemGameModeBase::PlayerDied);
	}
}

void ARPG_AnimationSystemGameModeBase::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

}

void ARPG_AnimationSystemGameModeBase::PlayerDied(ACharacter* Character)
{
	GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Cyan, TEXT("Broadcasted from GameMode"));
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);
}
