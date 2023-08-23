// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SinusoidalComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_ANIMATIONSYSTEM_API USinusoidalComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USinusoidalComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sinusoidal Behavior")
	bool bFloatingMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sinusoidal Behavior")
	bool bSwingMovement;

	// How wide the sinusoidal movement (considered as Maximum Degree for Swinging movement)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sinusoidal Behavior")
	float Amplitude = 5.f;

	// Time between each period
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sinusoidal Behavior")
	float TimeConstant = 5.f;

	// Whether should reverse the movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Sinusoidal Behavior")
	bool bShouldReversed = false;

	float RunningTime = 0.f;
};
