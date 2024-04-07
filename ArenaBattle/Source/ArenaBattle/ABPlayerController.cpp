// Fill out your copyright notice in the Description page of Project Settings.


#include "ABPlayerController.h"

void AABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UI 입력을 배제하고, 게임에만 입력을 전달하도록 하는 기능.
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}

void AABPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ABLOG_S(Warning);
}

void AABPlayerController::OnPossess(APawn* InPawn)
{
	ABLOG_S(Warning);
	Super::OnPossess(InPawn);
}
