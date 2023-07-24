// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RPG_AnimationSystemGameModeBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDiedSignature, ACharacter*, Character);

UCLASS()
class RPG_ANIMATIONSYSTEM_API ARPG_AnimationSystemGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	const FOnPlayerDiedSignature& GetOnPlayerDied() const { return OnPlayerDied; }

	// overriding RestartPlayer() on AGameModeBase class
	virtual void RestartPlayer(AController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void PlayerDied(ACharacter* Character);

protected:
	UPROPERTY()
	FOnPlayerDiedSignature OnPlayerDied;
	

};
