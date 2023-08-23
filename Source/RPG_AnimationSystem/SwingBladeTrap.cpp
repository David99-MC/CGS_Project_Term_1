// Fill out your copyright notice in the Description page of Project Settings.


#include "SwingBladeTrap.h"
#include "DealDamageComponent.h"
#include "SinusoidalComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ASwingBladeTrap::ASwingBladeTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PivotPoint = CreateDefaultSubobject<UBoxComponent>(TEXT("Pivot Point"));
	SetRootComponent(PivotPoint);

	SwingBlade = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Swing Blade"));
	SwingBlade->SetupAttachment(RootComponent);

	DealDamageComponent = CreateDefaultSubobject<UDealDamageComponent>(TEXT("Damage Component"));
	DealDamageComponent->GetHitboxArea()->SetupAttachment(SwingBlade);

	SinusoidalComponent = CreateDefaultSubobject<USinusoidalComponent>(TEXT("Sinusoidal Component"));

}

// Called when the game starts or when spawned
void ASwingBladeTrap::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwingBladeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

