// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UPlayground/Managers/PGGameFeatureManagerAction.h"
#include "PGRPGManagersFeature.generated.h"

/**
 * RPG 매니저 기능
 * 게임 피처 플러그인으로 사용될 RPG 매니저 시스템을 정의합니다.
 */
UCLASS(BlueprintType, Blueprintable)
class UPLAYGROUND_API UPGRPGManagersFeature : public UPGGameFeatureManagerAction
{
	GENERATED_BODY()

public:
	UPGRPGManagersFeature();
};
