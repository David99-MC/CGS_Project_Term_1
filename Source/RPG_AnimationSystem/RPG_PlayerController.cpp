// Fill out your copyright notice in the Description page of Project Settings.


#include "RPG_PlayerController.h"
#include "Blueprint/UserWidget.h"

void ARPG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARPG_PlayerController::DisplayHealthBar()
{
	if (HUDOverlayAsset)
	{
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}

	if (HUDOverlay == nullptr) return;

	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);
}
