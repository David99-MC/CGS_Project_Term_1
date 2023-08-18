// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_Character.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"

#include "RPG_CharacterMovementComponent.h"

#include "RPG_AnimationSystemGameModeBase.h"
#include "GameFramework/PlayerController.h"

#include "GameFrameWork/DamageType.h"

#include "HealthComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "CombatComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARPG_Character::ARPG_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<URPG_CharacterMovementComponent>(ARPG_Character::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(26.f, 88.f);

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

	RPG_CharacterMovementComponent->bOrientRotationToMovement = true;
	RPG_CharacterMovementComponent->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	RPG_CharacterMovementComponent->bUseControllerDesiredRotation = true;
	RPG_CharacterMovementComponent->bIgnoreBaseRotation = true;
	RPG_CharacterMovementComponent->MaxWalkSpeed = JogSpeed;
	RPG_CharacterMovementComponent->JumpZVelocity = 750.f;
	RPG_CharacterMovementComponent->AirControl = 0.35f;
	RPG_CharacterMovementComponent->GravityScale = 1.75f;
	RPG_CharacterMovementComponent->BrakingDecelerationWalking = 2000.f;

	RPG_CharacterMovementComponent->NavAgentProps.bCanCrouch = true; // Enable Crouch ability
	RPG_CharacterMovementComponent->MaxWalkSpeedCrouched = CrouchSpeed;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health"));

	CharacterCombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Big Sword"));
	Weapon->SetupAttachment(GetMesh(), FName("Weapon_RSocket"));

	WeaponHitbox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Hitbox"));
	WeaponHitbox->SetupAttachment(Weapon);

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

	WeaponHitbox->OnComponentBeginOverlap.AddDynamic(this, &ARPG_Character::OnWeaponHitboxBeginOverlap);

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
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ARPG_Character::Attack);
	}
	
}

void ARPG_Character::FellOutOfWorld(const UDamageType& DmgType)
{
	RespawnPlayer();
}

void ARPG_Character::Move(const FInputActionValue& Value)
{
	if (RPG_CharacterMovementComponent == nullptr) return;

	if (RPG_CharacterMovementComponent->IsClimbing())
	{
		HandleClimbMovement(Value);
	}
	else
	{
		HandleGroundMovement(Value);
	}
}

void ARPG_Character::HandleGroundMovement(const FInputActionValue& Value)
{
	FVector2d MovementVector = Value.Get<FVector2D>();

	// Get the forward and right vector from controller's rotation
	FRotator ControlRotationYaw(0.f, GetControlRotation().Yaw, 0.f);
	FVector ForwardVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::X);
	FVector RightVector = FRotationMatrix(ControlRotationYaw).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector, MovementVector.Y); // Move forward
	AddMovementInput(RightVector, MovementVector.X); // Move right
}

void ARPG_Character::HandleClimbMovement(const FInputActionValue& Value)
{
	FVector2d MovementVector = Value.Get<FVector2D>();

	// Right (Left in Unreal) hand rule when using CrossProduct to determine the direction
	// Forward vector means climbing up/down
	FVector ForwardVector = FVector::CrossProduct(
		-RPG_CharacterMovementComponent->GetClimbableSurfaceNormal(),
		GetActorRightVector()
	);
	// Right vector means climbing left/right
	FVector RightVector = FVector::CrossProduct(
		-RPG_CharacterMovementComponent->GetClimbableSurfaceNormal(),
		-GetActorUpVector()
	);

	AddMovementInput(ForwardVector, MovementVector.Y);
	AddMovementInput(RightVector, MovementVector.X); 
}

void ARPG_Character::Look(const FInputActionValue& Value)
{
	FVector2d LookDirection = Value.Get<FVector2d>();
	AddControllerPitchInput(LookDirection.Y);
	AddControllerYawInput(LookDirection.X);
}

void ARPG_Character::Sprint(const FInputActionValue& Value)
{
	if (RPG_CharacterMovementComponent && RPG_CharacterMovementComponent->IsClimbing())
		return;

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

void ARPG_Character::Attack(const FInputActionValue& Value)
{
	if (CharacterCombatComponent == nullptr) return;

	CharacterCombatComponent->StartAttack();
}

float ARPG_Character::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent == nullptr) return 0.f;

	float DamageTaken = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	HealthComponent->ReceiveDamage(DamageTaken);
	if (HealthComponent->IsDead())
	{
		RespawnPlayer();
	}

	return DamageTaken;
}

void ARPG_Character::RespawnPlayer()
{
	if (UWorld* World = GetWorld())
	{
		if (ARPG_AnimationSystemGameModeBase* GameMode = Cast<ARPG_AnimationSystemGameModeBase>(World->GetAuthGameMode()))
		{
			GameMode->RequestRespawnPlayer(this, Controller);
		}
	}
}

void ARPG_Character::OnWeaponHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (CharacterCombatComponent == nullptr || OtherActor == this) return;

	UGameplayStatics::ApplyDamage(OtherActor, 
								  CharacterCombatComponent->BaseDamage, 
								  Controller, 
								  this, 
								  UDamageType::StaticClass());
	
	if (CharacterCombatComponent->AttackHitParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this,
												 CharacterCombatComponent->AttackHitParticle,
												 OtherActor->GetActorLocation(),
												 OtherActor->GetActorRotation(),
												 true);
	}
}
