// Fill out your copyright notice in the Description page of Project Settings.


#include "MMGameModeBase.h"
#include "GameObject/Player/MMCharacterPlayer.h"
#include "GameObject/Player/MMPlayerController.h"

AMMGameModeBase::AMMGameModeBase()
{
	DefaultPawnClass = AMMCharacterPlayer::StaticClass();
	PlayerControllerClass = AMMPlayerController::StaticClass();
}
