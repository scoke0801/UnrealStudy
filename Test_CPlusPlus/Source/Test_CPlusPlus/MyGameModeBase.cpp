// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyCharacter::StaticClass();

	static ConstructorHelpers::FClassFinder<ACharacter> BP_Char(TEXT("/Script/Engine.Blueprint'/Game/BluePrints/BP_MyCharacter.BP_MyCharacter_C'"));

	if (BP_Char.Succeeded()) {
		DefaultPawnClass = BP_Char.Class;
	}
}
