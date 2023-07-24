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
class URPG_CharacterMovementComponent;
class UDamageType;

UCLASS()
class RPG_ANIMATIONSYSTEM_API ARPG_Character : public ACharacter
{
	GENERATED_BODY()

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

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* ClimbAction;

public:
	ARPG_Character(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	virtual void Destroyed();

	// Called when the Character fell below Z Kill
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);

	void ToggleCrouch(const FInputActionValue& Value);

	void Climb(const FInputActionValue& Value);

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Movement)
	URPG_CharacterMovementComponent* RPG_CharacterMovementComponent;



	/** Locomotion Variables */
	UPROPERTY(EditAnywhere, category = Movement)
	float JogSpeed = 400.f;

	UPROPERTY(EditAnywhere, category = Movement)
	float SprintSpeed = 650.f;

	UPROPERTY(EditAnywhere, category = Movement)
	float CrouchSpeed = 250.f;

	bool bIsSprinting;


};
