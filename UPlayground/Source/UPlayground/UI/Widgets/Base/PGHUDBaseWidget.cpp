#include "UI/Widgets/Base/PGHUDBaseWidget.h"

UPGHUDBaseWidget::UPGHUDBaseWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGHUDBaseWidget::SetElementID(const FName& InElementID)
{
    ElementID = InElementID;
}

FName UPGHUDBaseWidget::GetElementID() const
{
    return ElementID;
}

void UPGHUDBaseWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    UpdateWidget();
}

void UPGHUDBaseWidget::UpdateWidget()
{
    // 자식 클래스에서 오버라이드
}