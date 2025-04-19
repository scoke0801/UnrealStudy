// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "IA_FreeCameraMove.generated.h"

/**
 * 자유시점 카메라 이동을 위한 입력 액션 클래스
 */
UCLASS()
class UPLAYGROUND_API UIA_FreeCameraMove : public UInputAction
{
    GENERATED_BODY()

public:
    UIA_FreeCameraMove()
    {
        // 기본 설정
        ValueType = EInputActionValueType::Axis3D;
        bConsumeInput = true;
    }
};
