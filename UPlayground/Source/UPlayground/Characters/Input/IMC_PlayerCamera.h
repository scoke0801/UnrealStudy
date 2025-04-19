// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "IMC_PlayerCamera.generated.h"

/**
 * 플레이어 카메라 제어를 위한 입력 매핑 컨텍스트 클래스
 */
UCLASS()
class UPLAYGROUND_API UIMC_PlayerCamera : public UInputMappingContext
{
    GENERATED_BODY()

public:
    UIMC_PlayerCamera();
};
