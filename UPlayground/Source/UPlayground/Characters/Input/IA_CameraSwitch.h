// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "IA_CameraSwitch.generated.h"

/**
 * 카메라 모드 전환을 위한 입력 액션 클래스
 */
UCLASS()
class UPLAYGROUND_API UIA_CameraSwitch : public UInputAction
{
    GENERATED_BODY()

public:
    UIA_CameraSwitch()
    {
        // 기본 설정
        ValueType = EInputActionValueType::Boolean;
        bConsumeInput = true;
    }
};
