// Copyright Epic Games, Inc. All Rights Reserved.


#include "RPG_AnimationSystemGameModeBase.h"
#include "RPG_Character.h"

#include "RPG_PlayerController.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"


void ARPG_AnimationSystemGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}

void ARPG_AnimationSystemGameModeBase::RequestRespawnPlayer(ACharacter* EliminatedCharacter, AController* EliminatedController)
{
	if (EliminatedCharacter == nullptr || EliminatedController == nullptr) return;

	EliminatedCharacter->Reset();
	EliminatedCharacter->Destroy();

	TArray<AActor*> PlayerStartLocations;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStartLocations);

	if (PlayerStartLocations.Num() != 0)
	{
		RestartPlayerAtPlayerStart(EliminatedController, PlayerStartLocations[0]);
	}

}
