// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "CombatComponent.generated.h"


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
	
	UFUNCTION()
	void ShouldMontageStop(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload); // Callback for OnPlayMontageNotifyBegin delegate

public:
	UAnimInstance* OwningCharacterAnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsPlayer = false;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackComboMontage;

	int AttackIndex = 0;
		
};
