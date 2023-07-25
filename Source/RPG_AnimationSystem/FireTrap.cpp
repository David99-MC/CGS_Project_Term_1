// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTrap.h"
#include "DealDamageComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AFireTrap::AFireTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DealDamageComponent = CreateDefaultSubobject<UDealDamageComponent>(TEXT("Deal Damage Component"));
	if (DealDamageComponent && DealDamageComponent->GetCapsuleComponent())
	{
		RootComponent = DealDamageComponent->GetCapsuleComponent();
	}

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Effect"));
	ParticleEffect->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AFireTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

