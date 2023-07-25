// Fill out your copyright notice in the Description page of Project Settings.


#include "DealDamageComponent.h"
#include "Components/CapsuleComponent.h"
#include "RPG_Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UDealDamageComponent::UDealDamageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TriggerArea = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Area"));

}


// Called when the game starts
void UDealDamageComponent::BeginPlay()
{
	Super::BeginPlay();

	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &UDealDamageComponent::OnBeginOverlap);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &UDealDamageComponent::OnEndOverlap);
}

// Called every frame
void UDealDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Simulating DoT
	if (Player != nullptr)
	{
		UGameplayStatics::ApplyDamage(Player, DeltaTime * DamageRate, nullptr, GetOwner(), UDamageType::StaticClass());
	}
}

void UDealDamageComponent::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARPG_Character* OverlappingCharacter = Cast<ARPG_Character>(OtherActor))
	{
		Player = OverlappingCharacter;
	}
}

void UDealDamageComponent::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ARPG_Character* OverlappingCharacter = Cast<ARPG_Character>(OtherActor))
	{
		Player = nullptr;
	}
}


