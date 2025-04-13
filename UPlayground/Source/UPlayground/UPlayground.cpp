// Copyright Epic Games, Inc. All Rights Reserved.

#include "UPlayground.h"
#include "Modules/ModuleManager.h"

void FUPlaygroundModule::StartupModule()
{
    UE_LOG(LogTemp, Log, TEXT("UPlayground 모듈 시작"));
}

void FUPlaygroundModule::ShutdownModule()
{
    UE_LOG(LogTemp, Log, TEXT("UPlayground 모듈 종료"));
}

IMPLEMENT_PRIMARY_GAME_MODULE(FUPlaygroundModule, UPlayground, "UPlayground");
