#include "PGEnhancedInputComponent.h"

UPGEnhancedInputComponent::UPGEnhancedInputComponent()
{
    // 기본 생성자
}

void UPGEnhancedInputComponent::SetInputPriority(int32 NewPriority)
{
    // 이 컴포넌트의 입력 우선순위 설정
    Priority = NewPriority;
}

// 템플릿 특수화를 피하기 위해 실제 BindAction 구현은 헤더 파일에 인라인으로 정의되어 있음
