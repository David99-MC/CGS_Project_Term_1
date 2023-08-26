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

#pragma region Get Key Objective

	UFUNCTION(BlueprintCallable)
	void UpdateGetKeyObjectiveText();

	void InteractWithChestObjective();

	bool bHasFinishedGetKeyObjective = false;

#pragma endregion

#pragma region Open Gates Objective

	UFUNCTION(BlueprintCallable)
	void UpdateOpenGatesObjectiveText();

	void InteractWithGatesObjective();

	bool bHasFinishedOpenGatesObjective = false;

#pragma endregion

#pragma region Final Pickup Power Stone Objective

	UFUNCTION(BlueprintCallable)
	void UpdatePickupPowerStoneObjectiveText();

	void InteractWithPowerStoneObjective();

	bool bHasFinishedPickupPowerStoneObjective = false;

#pragma endregion

	// Parameter: Name of the Callback in String
	void PlayHUDFadeAnimation(FName CallBackName);

	void UpdateObjectiveNameText(FText TextToSet);

	// Pause the game and ask whether the player want to continue playing or play again
	UFUNCTION(BlueprintCallable)
	void SetGamePause(bool bCanPause);

public:
	UPROPERTY(EditDefaultsOnly, category = Widgets)
	TSubclassOf<UUserWidget> HUDOverlayClass;

	UPROPERTY()
	UUserWidget* HUDOverlay;

	UPROPERTY(EditDefaultsOnly, category = Widgets)
	TSubclassOf<UUserWidget> ObjectiveHUDClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* ObjectiveHUD;

	UPROPERTY(EditDefaultsOnly, category = Widgets)
	TSubclassOf<UUserWidget> PlayAgainHUDClass;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* PlayAgainHUD;

	TArray<AActor*> ObjectiveSkeletons;
};
