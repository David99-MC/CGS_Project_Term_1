// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveHUD.generated.h"

class UWidgetAnimation;
class UTextBlock;

UCLASS()
class RPG_ANIMATIONSYSTEM_API UObjectiveHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetObjectiveNameText(FText TextToSet);

	UFUNCTION(BlueprintCallable)
	void SetObjectiveText(FText TextToSet);

public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ObjectiveNameText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UTextBlock* ObjectiveText;

	
};
