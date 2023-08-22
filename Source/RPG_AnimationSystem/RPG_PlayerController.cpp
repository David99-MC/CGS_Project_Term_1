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

void ARPG_PlayerController::DecreaseObjectiveSkeleton()
{
	if (ObjectiveSkeletons.IsEmpty()) return;

	float LastIdx = ObjectiveSkeletons.Num() - 1;
	ObjectiveSkeletons.RemoveAt(LastIdx);

	if (ObjectiveSkeletons.IsEmpty())
	{
		// fade out
		UpdateObjectiveHUD(true);
		
		// Set the text and fade in
		FLatentActionInfo LatentActionInfo1;
		LatentActionInfo1.CallbackTarget = this;
		LatentActionInfo1.ExecutionFunction = "UpdateObjectiveText";
		LatentActionInfo1.Linkage = 1;
		UKismetSystemLibrary::Delay(this, .75f, LatentActionInfo1);

	}
}

void ARPG_PlayerController::UpdateObjectiveText()
{
	if (UObjectiveHUD* ObjectiveHUDOverlay = Cast<UObjectiveHUD>(ObjectiveHUD))
	{
		ObjectiveHUDOverlay->SetObjectiveText(FText::FromString("Get the key up from the ladder"));
	}
	UpdateObjectiveHUD(false);
}


