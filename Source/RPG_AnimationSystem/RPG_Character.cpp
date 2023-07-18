// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "RPG_CharacterMovementComponent.h"
#include "Engine/Engine.h"


// Sets default values
ARPG_Character::ARPG_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URPG_CharacterMovementComponent>(ARPG_Character::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 450.f;
	CameraBoom->bEnableCameraLag = true; // Smoothly move the camera to the half eye height when start crouching
	CameraBoom->CameraLagSpeed = 5.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	RPG_CharacterMovementComponent = Cast<URPG_CharacterMovementComponent>(GetCharacterMovement());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	GetCharacterMovement()->JumpZVelocity = 750.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->GravityScale = 1.75f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true; // Enable Crouch ability
	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchSpeed;

}

// Called when the game starts or when spawned
void ARPG_Character::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* LocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			LocalPlayerSubsystem->AddMappingContext(LocomotionMappingContext, 0);
		}
	}
	
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

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPG_Character::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPG_Character::Look);

		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &ARPG_Character::Sprint);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ARPG_Character::ToggleCrouch);

		EnhancedInputComponent->BindAction(ClimbAction, ETriggerEvent::Started, this, &ARPG_Character::Climb);
	}

}

void ARPG_Character::Move(const FInputActionValue& Value)
{
	FVector2d MovementVector = Value.Get<FVector2D>();

	// Get the forward vector from controller's rotation
	FRotator ControlRotationYaw(0.f, GetControlRotation().Yaw, 0.f);
	FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.Y); // Move forward
	AddMovementInput(RightVector, MovementVector.X); // Move right
}

void ARPG_Character::Look(const FInputActionValue& Value)
{
	FVector2d LookDirection = Value.Get<FVector2d>();
	AddControllerYawInput(LookDirection.X);
	AddControllerPitchInput(LookDirection.Y);
}

void ARPG_Character::Sprint(const FInputActionValue& Value)
{
	bIsSprinting = Value.Get<bool>();
	if (bIsSprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		bUseControllerRotationYaw = false;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
		bUseControllerRotationYaw = true;
	}
}

void ARPG_Character::ToggleCrouch(const FInputActionValue& Value)
{
	if (!bIsCrouched && !GetCharacterMovement()->IsFalling())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void ARPG_Character::Climb(const FInputActionValue& Value)
{
	if (RPG_CharacterMovementComponent == nullptr) return;

	if (RPG_CharacterMovementComponent->IsClimbing()) // Wants to stop climbing
	{
		RPG_CharacterMovementComponent->ToggleClimbing(false);
	}
	else 
	{
		RPG_CharacterMovementComponent->ToggleClimbing(true);
	}
}

