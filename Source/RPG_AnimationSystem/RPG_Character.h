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
class UHealthComponent;
class UBoxComponent;
class UStaticMeshComponent;
class UCombatComponent;
class UDamagedCameraShake;

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

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, category = Input)
	UInputAction* PauseGameAction;

public:
	ARPG_Character(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

	// Called when the Character fell below Z Kill
	virtual void FellOutOfWorld(const UDamageType& DmgType) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Move(const FInputActionValue& Value);

	void HandleGroundMovement(const FInputActionValue& Value);

	void HandleClimbMovement(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Sprint(const FInputActionValue& Value);

	void ToggleCrouch(const FInputActionValue& Value);

	void Climb(const FInputActionValue& Value);

	void Attack(const FInputActionValue& Value);

	void Interact(const FInputActionValue& Value);

	void PauseGame(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void RespawnPlayer();

	UFUNCTION()
	void OnWeaponHitboxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void PauseAnimation(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// true means transferring the weapon to the back, or back to the hand if otherwise
	void ChangeWeaponSocket(bool bToTheBack);

public:
	/* Essential Components */
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Movement)
	URPG_CharacterMovementComponent* RPG_CharacterMovementComponent;

	/* Combat Components*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Combat)
	UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat)
	UStaticMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Combat)
	UCombatComponent* CharacterCombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = Combat)
	UBoxComponent* WeaponHitbox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Animation)
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category = Animation)
	UAnimMontage* DeathMontage;

	UPROPERTY(EditDefaultsOnly, category = Combat)
	TSubclassOf<UDamagedCameraShake> DamagedCameraShakeClass;

	/** Locomotion Variables */
	UPROPERTY(EditAnywhere, category = Movement)
	float JogSpeed = 400.f;

	UPROPERTY(EditAnywhere, category = Movement)
	float SprintSpeed = 650.f;

	UPROPERTY(EditAnywhere, category = Movement)
	float CrouchSpeed = 250.f;

	bool bIsSprinting;

	bool bCanTurn = true;

	UPROPERTY(VisibleAnywhere)
	AActor* OverlappingActor;

	bool bIsGamePaused = false;
};
