// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CombatComponent.generated.h"

class UParticleSystem;
class USoundCue;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_ANIMATIONSYSTEM_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void StartAttack();
protected:
	virtual void BeginPlay() override;

	bool CheckValidPtr();
	
	// Callback for OnPlayMontageNotifyBegin delegate
	UFUNCTION()
	void ShouldMontageStop(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload); 

public:
	UAnimInstance* OwningCharacterAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsPlayer = false;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackComboMontage;

	int AttackIndex = 0;

	UPROPERTY(EditAnywhere)
	UParticleSystem* AttackHitParticle;

	UPROPERTY(EditAnywhere)
	USoundCue* AttachHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseDamage = 50.f;
		
};
