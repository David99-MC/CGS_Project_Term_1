// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "CombatComponent.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyCombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));

	EnemyHealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddDynamic(this, &AEnemy::StopPlayingAnimation);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (EnemyHealthComponent == nullptr || EnemyHealthComponent->IsDead()) return 0.f;

	float ActualAmount = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	EnemyHealthComponent->ReceiveDamage(ActualAmount);

	if (UAnimInstance* EnemyAnimInstance = GetMesh()->GetAnimInstance())
	{
		EnemyAnimInstance->Montage_Play(HitReactMontage);
		if (EnemyHealthComponent->IsDead())
		{
			EnemyAnimInstance->Montage_Play(DeathMontage);
		}
	}
	return ActualAmount;
}

void AEnemy::StopPlayingAnimation(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Delay a few seconds before destroying the actor
	FLatentActionInfo LatentActionInfo;
	LatentActionInfo.CallbackTarget = this;
	LatentActionInfo.ExecutionFunction = "DestroyActor";
	LatentActionInfo.Linkage = 1;

	UKismetSystemLibrary::Delay(this, 3.f,LatentActionInfo);
}

void AEnemy::DestroyActor()
{
	Destroy();
}

