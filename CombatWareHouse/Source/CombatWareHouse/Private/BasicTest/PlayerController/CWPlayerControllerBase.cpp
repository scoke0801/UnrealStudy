// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/PlayerController/CWPlayerControllerBase.h"
#include "BasicTest/UI/CWUIHUDIngame.h"

ACWPlayerControllerBase::ACWPlayerControllerBase()
{
	static ConstructorHelpers::FClassFinder<UCWUIHUDIngame> HUDWidegRef(TEXT("/Game/Blueprint/UI/WBP_IngameHUD.WBP_IngameHUD_C"));
	if(HUDWidegRef.Class)
	{
		_HUDWidgetClass = HUDWidegRef.Class;
	}
}

void ACWPlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);

	if (_HUDWIdget = CreateWidget<UCWUIHUDIngame>(this, _HUDWidgetClass))
	{
		_HUDWIdget->AddToViewport();
	}
}