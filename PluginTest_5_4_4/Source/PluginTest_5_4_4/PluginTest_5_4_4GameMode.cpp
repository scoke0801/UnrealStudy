// Copyright Epic Games, Inc. All Rights Reserved.

#include "PluginTest_5_4_4GameMode.h"
#include "PluginTest_5_4_4Character.h"
#include "UObject/ConstructorHelpers.h"

APluginTest_5_4_4GameMode::APluginTest_5_4_4GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
