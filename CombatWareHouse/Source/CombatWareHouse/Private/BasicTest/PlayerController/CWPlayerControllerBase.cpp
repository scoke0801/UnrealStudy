// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/PlayerController/CWPlayerControllerBase.h"


void ACWPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);
}