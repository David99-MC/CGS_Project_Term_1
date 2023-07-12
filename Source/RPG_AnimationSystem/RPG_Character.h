// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "RPG_Character.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class RPG_ANIMATIONSYSTEM_API ARPG_Character : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(EditAnywhere, category = Input)
	UInputMappingContext* LocomotionMappingContext;

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* JumpAction;

public:
	ARPG_Character();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);

	void CrouchButtonPressed(const FInputActionValue& Value);

	//void Jump(const FInputActionValue& Value);

public:
	UPROPERTY(EditAnywhere, category = Movement)
	float JogSpeed = 400.f;

	UPROPERTY(EditAnywhere, category = Movement)
	float SprintSpeed = 650.f;

	UPROPERTY(EditAnywhere, category = Movement)
	float CrouchSpeed = 300.f;

public:
	bool bIsSprinting;


};
