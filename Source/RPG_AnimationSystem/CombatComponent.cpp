// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "RPG_Character.h"
#include "Enemy.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (AEnemy* EnemyOwner = Cast<AEnemy>(GetOwner()))
	{
		OwningCharacterAnimInstance = EnemyOwner->GetMesh()->GetAnimInstance();
	}
	else if (ARPG_Character* PlayerOwner = Cast<ARPG_Character>(GetOwner()))
	{
		OwningCharacterAnimInstance = PlayerOwner->GetMesh()->GetAnimInstance();
	}

	if (bIsPlayer)
	{
		OwningCharacterAnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &UCombatComponent::ShouldMontageStop);
	}
	
}

// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::ShouldMontageStop(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	if (!CheckValidPtr()) return;

	AttackIndex -= 1;
	if (AttackIndex < 0)
	{
		OwningCharacterAnimInstance->Montage_Stop(.25f, AttackComboMontage);
		AttackIndex = 0; // reset attack count
	}

}

void UCombatComponent::StartAttack()
{
	if (!CheckValidPtr()) return;

	if (OwningCharacterAnimInstance->Montage_IsPlaying(AttackComboMontage) == false) // not attacking
	{
		OwningCharacterAnimInstance->Montage_Play(AttackComboMontage);
	}
	else
	{
		AttackIndex = 1;
	}
}

bool UCombatComponent::CheckValidPtr()
{
	return OwningCharacterAnimInstance != nullptr && AttackComboMontage != nullptr;
}

