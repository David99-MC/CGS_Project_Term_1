// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"

#include "BasePickup.generated.h"

class UBoxComponent;
class ARPG_PlayerController;
class ARPG_Character;

UCLASS()
class RPG_ANIMATIONSYSTEM_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePickup();

	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteInteraction();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTriggerAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTriggerAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* InteractArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleEffect;

	ARPG_PlayerController* PlayerController;

	// Timeline and curve for interpolation
	FTimeline Timeline;

	ARPG_Character* OverlappingPlayer = nullptr;

};
