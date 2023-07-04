// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Character.h"

// Sets default values
ARPG_Character::ARPG_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARPG_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARPG_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARPG_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

