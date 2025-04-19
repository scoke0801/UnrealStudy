// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "IA_FreeCameraRotate.generated.h"

/**
 * 자유시점 카메라 회전을 위한 입력 액션 클래스
 */
UCLASS()
class UPLAYGROUND_API UIA_FreeCameraRotate : public UInputAction
{
    GENERATED_BODY()

public:
    UIA_FreeCameraRotate()
    {
        // 기본 설정
        ValueType = EInputActionValueType::Axis2D;
        bConsumeInput = true;
    }
};
