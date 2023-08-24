// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePickup.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "RPG_Character.h"
#include "RPG_PlayerController.h"

// Sets default values
ABasePickup::ABasePickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TriggerArea = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger Area"));
	SetRootComponent(TriggerArea);

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	BaseMesh->SetupAttachment(RootComponent);

	AddOnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Add-on Mesh"));
	AddOnMesh->SetupAttachment(RootComponent);

	ParticleEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Effect"));
	ParticleEffect->SetupAttachment(RootComponent);
}

void ABasePickup::BeginPlay()
{
	Super::BeginPlay();
	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ABasePickup::OnTriggerAreaBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &ABasePickup::OnTriggerAreaEndOverlap);
}

void ABasePickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShouldStartOpen)
	{
		OpenChestLid(DeltaTime);
	}
}

void ABasePickup::ExecuteInteraction()
{
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, TEXT("Interaction Executed!"));
	if (PlayerController == nullptr) return;

	bShouldStartOpen = true;

	PlayerController->InteractWithChestObjective();
}


void ABasePickup::OnTriggerAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARPG_Character* Player = Cast<ARPG_Character>(OtherActor))
	{
		Player->OverlappingActor = this;
		PlayerController = Cast<ARPG_PlayerController>(Player->GetController());
	}
}

void ABasePickup::OnTriggerAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARPG_Character* Player = Cast<ARPG_Character>(OtherActor))
	{
		Player->OverlappingActor = nullptr;
	}
}

void ABasePickup::OpenChestLid(float DeltaTime)
{
	FRotator TargetRotation(0.f, 0.f, -70.f);
	FRotator InterpRotation =
		FMath::RInterpTo(AddOnMesh->GetComponentRotation(), TargetRotation, DeltaTime, 5.f);

	AddOnMesh->SetWorldRotation(InterpRotation);
}

