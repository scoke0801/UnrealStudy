#include "UI/Widgets/Base/PGHUDContainerWidget.h"
#include "UI/Widgets/Base/PGHUDBaseWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

UPGHUDContainerWidget::UPGHUDContainerWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGHUDContainerWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 레이어 패널이 생성되었는지 확인
    if (!BackgroundLayerPanel || !GameLayerPanel || !ForegroundLayerPanel || !ModalLayerPanel)
    {
        UE_LOG(LogTemp, Error, TEXT("HUD Container: Layer panels not properly set up in widget blueprint"));
    }
    
    // 코너 패널이 생성되었는지 확인
    if (!TopLeftPanel || !TopRightPanel || !BottomLeftPanel || !BottomRightPanel || !CenterPanel)
    {
        UE_LOG(LogTemp, Error, TEXT("HUD Container: Corner panels not properly set up in widget blueprint"));
    }
}

void UPGHUDContainerWidget::AddHUDElement(UPGHUDBaseWidget* Widget, EPGHUDLayer Layer)
{
    if (!Widget)
    {
        return;
    }
    
    UPanelWidget* TargetPanel = GetLayerPanel(Layer);
    if (TargetPanel)
    {
        // 이미 부모가 있으면 제거
        if (Widget->GetParent())
        {
            Widget->RemoveFromParent();
        }
        
        // 패널에 추가
        UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(TargetPanel->AddChild(Widget));
        if (PanelSlot)
        {
            // 기본적으로 전체 영역 차지
            PanelSlot->SetAnchors(FAnchors(0.0f, 0.0f, 1.0f, 1.0f));
            PanelSlot->SetOffsets(FMargin(0.0f, 0.0f, 0.0f, 0.0f));
            PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            
            // 위젯 활성화
            Widget->SetIsEnabled(true);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HUD Container: Failed to add widget to layer %d"), static_cast<int32>(Layer));
    }
}

UPanelWidget* UPGHUDContainerWidget::GetLayerPanel(EPGHUDLayer Layer) const
{
    switch (Layer)
    {
        case EPGHUDLayer::Background:
            return BackgroundLayerPanel;
        case EPGHUDLayer::GameLayer:
            return GameLayerPanel;
        case EPGHUDLayer::Foreground:
            return ForegroundLayerPanel;
        case EPGHUDLayer::Modal:
            return ModalLayerPanel;
        default:
            return nullptr;
    }
}

UPanelWidget* UPGHUDContainerWidget::GetCornerPanel(EPGHUDCornerPosition Position) const
{
    switch (Position)
    {
        case EPGHUDCornerPosition::TopLeft:
            return TopLeftPanel;
        case EPGHUDCornerPosition::TopRight:
            return TopRightPanel;
        case EPGHUDCornerPosition::BottomLeft:
            return BottomLeftPanel;
        case EPGHUDCornerPosition::BottomRight:
            return BottomRightPanel;
        case EPGHUDCornerPosition::Center:
            return CenterPanel;
        default:
            return nullptr;
    }
}

void UPGHUDContainerWidget::AddElementToCorner(UPGHUDBaseWidget* Widget, EPGHUDCornerPosition Position)
{
    if (!Widget)
    {
        return;
    }
    
    UPanelWidget* TargetPanel = GetCornerPanel(Position);
    if (TargetPanel)
    {
        // 이미 부모가 있으면 제거
        if (Widget->GetParent())
        {
            Widget->RemoveFromParent();
        }
        
        // 패널에 추가
        TargetPanel->AddChild(Widget);
        
        // 위젯 활성화
        Widget->SetIsEnabled(true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("HUD Container: Failed to add widget to corner position %d"), static_cast<int32>(Position));
    }
}