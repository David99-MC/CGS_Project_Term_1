// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveHUD.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"

void UObjectiveHUD::SetObjectiveNameText(FText TextToSet)
{
	ObjectiveNameText->SetText(TextToSet);
}

void UObjectiveHUD::SetObjectiveText(FText TextToSet)
{
	ObjectiveText->SetText(TextToSet);
}
