// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "ObjectiveHUD.h"
#include "Kismet/KismetSystemLibrary.h"


void ARPG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsWithTag(this, FName("Skeleton"), ObjectiveSkeletons);
}

void ARPG_PlayerController::DisplayHealthBar()
{
	if (HUDOverlayClass)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayClass);
	}

	if (HUDOverlay == nullptr) return;

	HUDOverlay->AddToViewport();
}

void ARPG_PlayerController::DisplayObjectiveHUD()
{
	if (ObjectiveHUDClass)
	{
		ObjectiveHUD = CreateWidget<UUserWidget>(this, ObjectiveHUDClass);
	}

	if (ObjectiveHUD == nullptr) return;

	ObjectiveHUD->AddToViewport();
}

// Interaction Objective
void ARPG_PlayerController::DecreaseObjectiveSkeleton()
{
	if (ObjectiveSkeletons.IsEmpty()) return;

	float LastIdx = ObjectiveSkeletons.Num() - 1;
	ObjectiveSkeletons.RemoveAt(LastIdx);

	if (ObjectiveSkeletons.IsEmpty())
	{
		bHasFinishedSkeletonObjective = true;
		PlayHUDFadeAnimation("UpdateSkeletonObjectiveText");
	}
}

void ARPG_PlayerController::InteractWithChestObjective()
{
	bHasFinishedGetKeyObjective = true;
	PlayHUDFadeAnimation("UpdateGetKeyObjectiveText");
}

void ARPG_PlayerController::InteractWithGatesObjective()
{
	bHasFinishedOpenGatesObjective = true;
	PlayHUDFadeAnimation("UpdateOpenGatesObjectiveText");
}

void ARPG_PlayerController::InteractWithPowerStoneObjective()
{
	bHasFinishedPickupPowerStoneObjective = true;
	PlayHUDFadeAnimation("UpdatePickupPowerStoneObjectiveText");
}

// Updating Objective Text
void ARPG_PlayerController::UpdateSkeletonObjectiveText()
{
	UpdateObjectiveNameText(FText::FromString("Get the key up from the ladder"));
}

void ARPG_PlayerController::UpdateGetKeyObjectiveText()
{
	UpdateObjectiveNameText(FText::FromString("Open the gates with the key"));
}

void ARPG_PlayerController::UpdateOpenGatesObjectiveText()
{
	UpdateObjectiveNameText(FText::FromString("Now HURRY and Pick up the Power Stone!"));
}

void ARPG_PlayerController::UpdatePickupPowerStoneObjectiveText()
{
	UpdateObjectiveNameText(FText::FromString("CONGRATULATIONS! YOU WON"));
}

void ARPG_PlayerController::PlayHUDFadeAnimation(FName CallBackName)
{
	// fade out
	UpdateObjectiveHUD(true);

	UObjectiveHUD* ObjectiveHUDOverlay = Cast<UObjectiveHUD>(ObjectiveHUD);
	if (bHasFinishedPickupPowerStoneObjective && ObjectiveHUDOverlay)
	{
		ObjectiveHUDOverlay->SetObjectiveText(FText::FromString(""));
	}

	// Set the text and fade in
	FLatentActionInfo LatentActionInfo1;
	LatentActionInfo1.CallbackTarget = this;
	LatentActionInfo1.ExecutionFunction = CallBackName;
	LatentActionInfo1.Linkage = 1;
	UKismetSystemLibrary::Delay(this, 1.f, LatentActionInfo1);
}

void ARPG_PlayerController::UpdateObjectiveNameText(FText TextToSet)
{
	if (UObjectiveHUD* ObjectiveHUDOverlay = Cast<UObjectiveHUD>(ObjectiveHUD))
	{
		ObjectiveHUDOverlay->SetObjectiveNameText(TextToSet);
	}
	UpdateObjectiveHUD(false); // Fade in with the newly set Objective Text

	SetGamePause(bHasFinishedPickupPowerStoneObjective);
}

void ARPG_PlayerController::SetGamePause(bool bCanPause)
{
	if (bCanPause)
	{
		if (PlayAgainHUDClass)
		{
			PlayAgainHUD = CreateWidget<UUserWidget>(this, PlayAgainHUDClass);
			PlayAgainHUD->AddToViewport();
			SetPause(true);
			SetShowMouseCursor(true);
			SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		if (PlayAgainHUD && PlayAgainHUD->IsInViewport())
		{
			PlayAgainHUD->RemoveFromParent();
			SetPause(false);
			SetShowMouseCursor(false);
			SetInputMode(FInputModeGameOnly());
		}
	}
}


