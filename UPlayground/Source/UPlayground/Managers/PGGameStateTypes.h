// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGameStateTypes.generated.h"

/**
 * 게임 상태 타입 열거형
 * 게임의 현재 상태를 나타냅니다.
 */
UENUM(BlueprintType)
enum class EPGGameStateType : uint8
{
	None             UMETA(DisplayName = "None"),
	Loading          UMETA(DisplayName = "Loading"),
	MainMenu         UMETA(DisplayName = "MainMenu"),
	GamePaused       UMETA(DisplayName = "GamePaused"),
	GameRunning      UMETA(DisplayName = "GameRunning"),
	CutScene         UMETA(DisplayName = "CutScene"),
	Dialog           UMETA(DisplayName = "Dialog"),
	Inventory        UMETA(DisplayName = "Inventory"),
	CharacterMenu    UMETA(DisplayName = "CharacterMenu"),
	GameOver         UMETA(DisplayName = "GameOver"),
	Victory          UMETA(DisplayName = "Victory")
};
