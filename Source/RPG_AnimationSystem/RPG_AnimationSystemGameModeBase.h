// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPG_AnimationSystemGameModeBase.generated.h"

/**
 * 
 */

UCLASS()
class RPG_ANIMATIONSYSTEM_API ARPG_AnimationSystemGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	// Calling RestartPlayerAtLocation() on AGameModeBase class
	virtual void RequestRespawnPlayer(ACharacter* NewCharacter, AController* NewPlayer);

protected:
	virtual void BeginPlay() override;
	

};
