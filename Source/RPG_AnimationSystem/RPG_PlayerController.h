// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPG_PlayerController.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class RPG_ANIMATIONSYSTEM_API ARPG_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void DisplayHealthBar();

public:
	UPROPERTY(EditDefaultsOnly, category = Widgets)
	TSubclassOf<UUserWidget> HUDOverlayAsset;

	UPROPERTY()
	UUserWidget* HUDOverlay;
};
