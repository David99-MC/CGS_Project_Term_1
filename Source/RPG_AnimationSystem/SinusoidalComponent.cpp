// Fill out your copyright notice in the Description page of Project Settings.


#include "SinusoidalComponent.h"

USinusoidalComponent::USinusoidalComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	
}

void USinusoidalComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USinusoidalComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	RunningTime += DeltaTime;

	if (bFloatingMovement)
	{
		float DeltaZ = Amplitude * (FMath::Sin(RunningTime * TimeConstant));
		GetOwner()->AddActorWorldOffset(FVector(0.f, 0.f, DeltaZ));
	}
	else if (bSwingMovement)
	{
		float DeltaPitch = Amplitude * FMath::Sin(RunningTime * TimeConstant);
			
		FRotator BaseRotator = bShouldReversed ? FRotator(-DeltaPitch, 90.f, 0.f) : FRotator(DeltaPitch, 90.f, 0.f);
		GetOwner()->SetActorRotation(BaseRotator);
	}
}

