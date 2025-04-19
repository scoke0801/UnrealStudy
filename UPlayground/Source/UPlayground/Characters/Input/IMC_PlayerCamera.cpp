// Copyright Epic Games, Inc. All Rights Reserved.

#include "IMC_PlayerCamera.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputAction.h"
#include "InputTriggers.h"
#include "InputModifiers.h"

UIMC_PlayerCamera::UIMC_PlayerCamera()
{
    // 입력 매핑 컨텍스트는 블루프린트에서 구성됩니다.
    // 여기에서는 기본값만 설정하며, 실제 키 매핑은 에디터에서 수행됩니다.
    
    // 예시 매핑 (주석 처리됨, 에디터에서 설정 필요)
    // 1. 카메라 줌 - 마우스 휠
    // 2. 카메라 모드 전환 - TAB 키
    // 3. 자유 시점 카메라 이동 - WASD + QE 조합
    // 4. 자유 시점 카메라 회전 - 마우스 이동
}
