// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveStone.h"
#include "SinusoidalComponent.h"

#include "Particles/ParticleSystemComponent.h"
#include "RPG_Character.h"
#include "RPG_PlayerController.h"

AInteractiveStone::AInteractiveStone()
{
	SinusoidalComponent = CreateDefaultSubobject<USinusoidalComponent>(TEXT("Sinusoidal Component"));
}

void AInteractiveStone::ExecuteInteraction()
{
	Super::ExecuteInteraction();
	if (!PlayerController->bHasFinishedOpenGatesObjective)
		return;

	if (!PlayerController->bHasFinishedPickupPowerStoneObjective)
		PlayerController->InteractWithPowerStoneObjective();
	else
		return;

	ParticleEffect->DeactivateSystem();
	Destroy();
}
