// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "RPG_Character.h"
#include "RPG_PlayerController.h"

// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	SetRootComponent(InteractArea);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Effect"));
	ParticleEffect->SetupAttachment(RootComponent);
}

void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	InteractArea->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OnTriggerAreaBeginOverlap);
	InteractArea->OnComponentEndOverlap.AddDynamic(this, &ABasePickup::OnTriggerAreaEndOverlap);
}

void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Timeline.TickTimeline(DeltaTime);
}

void ABasePickup::ExecuteInteraction()
{
	if (PlayerController == nullptr) return;
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Interaction Executed!"));
}


void ABasePickup::OnTriggerAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARPG_Character* Player = Cast<ARPG_Character>(OtherActor))
	{
		Player->OverlappingActor = this;
		OverlappingPlayer = Player;
		PlayerController = Cast<ARPG_PlayerController>(Player->GetController());
	}
}

void ABasePickup::OnTriggerAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARPG_Character* Player = Cast<ARPG_Character>(OtherActor))
	{
		Player->OverlappingActor = nullptr;
		OverlappingPlayer = nullptr;
	}
}



