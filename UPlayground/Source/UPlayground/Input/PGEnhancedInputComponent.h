#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "PGEnhancedInputComponent.generated.h"

/**
 * 향상된 입력 시스템을 위한 커스텀 입력 컴포넌트
 */
UCLASS()
class UPLAYGROUND_API UPGEnhancedInputComponent : public UEnhancedInputComponent
{
    GENERATED_BODY()
    
public:
    UPGEnhancedInputComponent();
    
    // 타입 안전성이 보장된 바인딩 헬퍼 메서드
    template<class UserClass, typename FuncType>
    void BindActionSafe(const UInputAction* Action, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func)
    {
        if (Action && Object)
        {
            UEnhancedInputComponent::BindAction(Action, TriggerEvent, Object, Func);
        }
    }
    
    template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
    void BindActionPressAndRelease(const UInputAction* Action, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc)
    {
        if (Action && Object)
        {
            UEnhancedInputComponent::BindAction(Action, ETriggerEvent::Started, Object, PressedFunc);
            UEnhancedInputComponent::BindAction(Action, ETriggerEvent::Completed, Object, ReleasedFunc);
        }
    }
    
    // 입력 우선순위 관리 헬퍼
    void SetInputPriority(int32 NewPriority);
};
