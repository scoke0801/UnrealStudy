// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

// API 매크로 정의
#ifndef UPLAYGROUND_API
#define UPLAYGROUND_API DLLEXPORT
#endif

class FUPlaygroundModule : public IModuleInterface
{
public:
    /** IModuleInterface 구현 */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
