// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwingBladeTrap.generated.h"

class UDealDamageComponent;
class USinusoidalComponent;

UCLASS()
class RPG_ANIMATIONSYSTEM_API ASwingBladeTrap : public AActor
{
	GENERATED_BODY()
	
public:	
	ASwingBladeTrap();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDealDamageComponent* DealDamageComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USinusoidalComponent* SinusoidalComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SwingBlade;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* PivotPoint;
};
