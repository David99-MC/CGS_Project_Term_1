// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPG_Character.generated.h"

UCLASS()
class RPG_ANIMATIONSYSTEM_API ARPG_Character : public ACharacter
{
	GENERATED_BODY()

public:
	ARPG_Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
