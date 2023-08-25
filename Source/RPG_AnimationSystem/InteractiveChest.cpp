// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveChest.h"
#include "Particles/ParticleSystemComponent.h"
#include "RPG_Character.h"
#include "RPG_PlayerController.h"

AInteractiveChest::AInteractiveChest()
{
	ChestLid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Chest Lid"));
	ChestLid->SetupAttachment(RootComponent);
}

void AInteractiveChest::BeginPlay()
{
	Super::BeginPlay();
	if (ChestCurveFloat)
	{
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindDynamic(this, &AInteractiveChest::OpenChestLid);
		
		Timeline.AddInterpFloat(ChestCurveFloat, TimelineProgress);
	}
}

void AInteractiveChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractiveChest::ExecuteInteraction()
{
	Super::ExecuteInteraction();

	if (!PlayerController->bHasFinishedSkeletonObjective) 
		return;

	if (!PlayerController->bHasFinishedGetKeyObjective)
		PlayerController->InteractWithChestObjective();
	else
		return;

	if (!bIsChestOpened) 
		Timeline.Play();

	bIsChestOpened = true;

	ParticleEffect->DeactivateSystem();
}

void AInteractiveChest::OpenChestLid(float CurveValue)
{
	FRotator TargetRotation(0.f, 0.f, CurveValue * LidOpenAngle);

	ChestLid->SetRelativeRotation(TargetRotation);
}


