// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "IA_CameraZoom.generated.h"

/**
 * 카메라 줌 인/아웃을 위한 입력 액션 클래스
 */
UCLASS()
class UPLAYGROUND_API UIA_CameraZoom : public UInputAction
{
    GENERATED_BODY()

public:
    UIA_CameraZoom()
    {
        // 기본 설정
        ValueType = EInputActionValueType::Axis1D;
        bConsumeInput = true;
    }
};
