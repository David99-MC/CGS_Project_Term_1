// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePickup.h"
#include "InteractiveChest.generated.h"

/**
 * 
 */
UCLASS()
class RPG_ANIMATIONSYSTEM_API AInteractiveChest : public ABasePickup
{
	GENERATED_BODY()

public:
	AInteractiveChest();

	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteInteraction() override;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OpenChestLid(float DeltaTime);

public:
	bool bIsChestOpened = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ChestLid;

	float LidOpenAngle = -70.f;

	UPROPERTY(EditAnywhere)
	UCurveFloat* ChestCurveFloat;
	
};
