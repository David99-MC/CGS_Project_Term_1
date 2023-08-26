// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "InteractiveStone.generated.h"

class USinusoidalComponent;

UCLASS()
class RPG_ANIMATIONSYSTEM_API AInteractiveStone : public ABasePickup
{
	GENERATED_BODY()

public:
	AInteractiveStone();

	virtual void ExecuteInteraction() override;

public:
	UPROPERTY(VisibleAnywhere)
	USinusoidalComponent* SinusoidalComponent;
	
	
};
