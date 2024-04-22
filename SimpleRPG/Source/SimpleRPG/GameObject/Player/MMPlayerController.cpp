// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObject/Player/MMPlayerController.h"

void AMMPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly gameOnlyInputMode;
	SetInputMode(gameOnlyInputMode);
}
