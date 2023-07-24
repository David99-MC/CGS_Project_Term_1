// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPG_AnimationSystemGameModeBase.h"
#include "RPG_Character.h"

#include "RPG_PlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


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

	TArray<AActor*> PlayerStartLocations;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStartLocations);

	RestartPlayerAtPlayerStart(NewPlayer, PlayerStartLocations[0]);

}

void ARPG_AnimationSystemGameModeBase::PlayerDied(ACharacter* Character)
{
	GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Cyan, TEXT("Broadcasted from GameMode"));
	ARPG_PlayerController* CharacterController = Cast<ARPG_PlayerController>(Character->GetController());
	RestartPlayer(CharacterController);
}
