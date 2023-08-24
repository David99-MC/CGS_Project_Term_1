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

#pragma region Skeleton Objective
	UFUNCTION(BlueprintCallable)
	void DisplayObjectiveHUD();

	// Play the fade in/out animation
	// Parameter: true means fade in
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateObjectiveHUD(bool bReversed);

	void DecreaseObjectiveSkeleton();

	UFUNCTION(BlueprintCallable)
	void UpdateSkeletonObjectiveText();

	bool bHasFinishedSkeletonObjective = false;

#pragma endregion

	UFUNCTION(BlueprintCallable)
	void UpdateGetKeyObjectiveText();

	void InteractWithChestObjective();

	void PlayHUDFadeAnimation(FName CallBackName);

public:
	UPROPERTY(EditDefaultsOnly, category = Widgets)
	TSubclassOf<UUserWidget> HUDOverlayClass;

	UPROPERTY()
	UUserWidget* HUDOverlay;

	UPROPERTY(EditDefaultsOnly, category = Widgets)
	TSubclassOf<UUserWidget> ObjectiveHUDClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* ObjectiveHUD;

	TArray<AActor*> ObjectiveSkeletons;
};
