// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasePickup.generated.h"

class USphereComponent;
class ARPG_PlayerController;

UCLASS()
class RPG_ANIMATIONSYSTEM_API ABasePickup : public AActor
{
	GENERATED_BODY()
	
public:	
	ABasePickup();

	virtual void Tick(float DeltaTime) override;

	void ExecuteInteraction();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTriggerAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTriggerAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void OpenChestLid(float DeltaTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* TriggerArea;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* AddOnMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UParticleSystemComponent* ParticleEffect;

	ARPG_PlayerController* PlayerController;

protected:
	bool bShouldStartOpen = false;

	
};
