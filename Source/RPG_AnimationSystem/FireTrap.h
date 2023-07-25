// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FireTrap.generated.h"

class UDealDamageComponent;
class UParticleSystemComponent;

UCLASS()
class RPG_ANIMATIONSYSTEM_API AFireTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireTrap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere)
	UDealDamageComponent* DealDamageComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* ParticleEffect;

};
