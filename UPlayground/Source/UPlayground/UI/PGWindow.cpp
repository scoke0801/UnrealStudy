// PGWindow.cpp
#include "PGWindow.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"

UPGWindow::UPGWindow(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bDraggable(true)
    , bResizable(false)
    , bIsDragging(false)
    , DragOffset(FVector2D::ZeroVector)
{
    // 창은 기본적으로 더 높은 Z-오더를 가짐
    ZOrder = 10;
}

void UPGWindow::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 바인딩된 위젯 초기화
    if (TitleText)
    {
        TitleText->SetText(WindowTitle);
    }
    
    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UPGWindow::OnCloseButtonClicked);
    }
}

void UPGWindow::SetWindowTitle(const FText& NewTitle)
{
    WindowTitle = NewTitle;
    
    if (TitleText)
    {
        TitleText->SetText(WindowTitle);
    }
}

void UPGWindow::SetDraggable(bool bInDraggable)
{
    bDraggable = bInDraggable;
}

void UPGWindow::SetResizable(bool bInResizable)
{
    bResizable = bInResizable;
}

FReply UPGWindow::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bDraggable && TitleBar && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        // 마우스 클릭이 타이틀바 영역에 있는지 확인
        FGeometry TitleBarGeometry = TitleBar->GetCachedGeometry();
        FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
        
        if (TitleBarGeometry.IsUnderLocation(MousePosition))
        {
            StartDrag(MousePosition);
            return FReply::Handled().CaptureMouse(TakeWidget());
        }
    }
    
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

FReply UPGWindow::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging && InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
    {
        EndDrag();
        return FReply::Handled().ReleaseMouseCapture();
    }
    
    return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

FReply UPGWindow::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    if (bIsDragging)
    {
        FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
        FVector2D NewPosition = MousePosition - DragOffset;
        
        // 캔버스 슬롯을 통해 위젯 위치 설정
        UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
        if (CanvasSlot)
        {
            CanvasSlot->SetPosition(NewPosition);
        }
        
        return FReply::Handled();
    }
    
    return Super::NativeOnMouseMove(InGeometry, InMouseEvent);
}

void UPGWindow::StartDrag(const FVector2D& InMousePosition)
{
    bIsDragging = true;
    
    // 현재 위젯의 위치를 얻음
    UCanvasPanelSlot* CanvasSlot = UWidgetLayoutLibrary::SlotAsCanvasSlot(this);
    if (CanvasSlot)
    {
        FVector2D CurrentPosition = CanvasSlot->GetPosition();
        DragOffset = InMousePosition - CurrentPosition;
    }
}

void UPGWindow::EndDrag()
{
    bIsDragging = false;
}

void UPGWindow::OnCloseButtonClicked()
{
    HideWidget();
}