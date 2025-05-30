// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DealDamageComponent.generated.h"

class UBoxComponent;
class ARPG_Character;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_ANIMATIONSYSTEM_API UDealDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDealDamageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override; 

	UBoxComponent* GetHitboxArea() const { return HitboxArea; }

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* HitboxArea;

	ARPG_Character* Player = nullptr;

	UPROPERTY(EditAnywhere, category = "General Setting")
	bool bCanDealDoT;

	UPROPERTY(EditAnywhere, category = FireTrapDamage)
	float FireDamageRate = 5.f;

	UPROPERTY(EditAnywhere, category = SwingBladeDamage)
	float SwingBladeDamage = 50.f;
		
};
