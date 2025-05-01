#include "UI/Widgets/Elements/PGMinimapWidget.h"
#include "UI/Widgets/Elements/PGMinimapMarkerWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Blueprint/WidgetTree.h"

UPGMinimapWidget::UPGMinimapWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGMinimapWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 버튼 이벤트 바인딩
    if (ZoomInButton)
    {
        ZoomInButton->OnClicked.AddDynamic(this, &UPGMinimapWidget::OnZoomInClicked);
    }
    
    if (ZoomOutButton)
    {
        ZoomOutButton->OnClicked.AddDynamic(this, &UPGMinimapWidget::OnZoomOutClicked);
    }
}

void UPGMinimapWidget::UpdateWidget()
{
    Super::UpdateWidget();
    
    // 초기 상태 설정
    SetMinimapZoom(CurrentZoom);
}

void UPGMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // 필요시 미니맵 업데이트 (플레이어 위치 자동 갱신 등)
}

void UPGMinimapWidget::SetMinimapTexture(UTexture2D* MapTexture)
{
    if (MinimapImage && MapTexture)
    {
        MinimapImage->SetBrushFromTexture(MapTexture);
    }
}

void UPGMinimapWidget::SetPlayerPosition(const FVector2D& Position, float Rotation)
{
    if (!PlayerMarker || !MarkersContainer)
    {
        return;
    }
    
    // 플레이어 마커 위치 조정
    UCanvasPanelSlot* PlayerSlot = Cast<UCanvasPanelSlot>(PlayerMarker->Slot);
    if (PlayerSlot)
    {
        // 위치 설정 (미니맵 좌표계로 변환 필요)
        PlayerSlot->SetPosition(Position);
        
        // 회전 설정
        PlayerMarker->SetRenderTransformAngle(Rotation);
    }
}

void UPGMinimapWidget::SetMinimapZoom(float ZoomLevel)
{
    // 범위 내로 보정
    CurrentZoom = FMath::Clamp(ZoomLevel, MinZoom, MaxZoom);
    
    // 미니맵 이미지 스케일 조정
    if (MinimapImage)
    {
        MinimapImage->SetRenderScale(FVector2D(CurrentZoom, CurrentZoom));
    }
}

void UPGMinimapWidget::AddMarker(FName MarkerID, const FVector2D& Position, UTexture2D* MarkerIcon, const FLinearColor& MarkerColor)
{
    // 이미 존재하는 마커인지 확인
    if (MarkerWidgets.Contains(MarkerID))
    {
        // 위치만 업데이트
        UpdateMarkerPosition(MarkerID, Position);
        return;
    }
    
    // 마커 컨테이너 확인
    if (!MarkersContainer || !MarkerWidgetClass)
    {
        return;
    }
    
    // 새 마커 위젯 생성
    UPGMinimapMarkerWidget* MarkerWidget = CreateWidget<UPGMinimapMarkerWidget>(this, MarkerWidgetClass);
    if (MarkerWidget)
    {
        // 마커 설정
        MarkerWidget->SetMarkerData(MarkerID, MarkerIcon, MarkerColor);
        
        // 캔버스에 추가
        UCanvasPanelSlot* MarkerSlot = MarkersContainer->AddChildToCanvas(MarkerWidget);
        if (MarkerSlot)
        {
            // 위치 설정
            MarkerSlot->SetPosition(Position);
            MarkerSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            MarkerSlot->SetAutoSize(true);
        }
        
        // 마커 위젯 저장
        MarkerWidgets.Add(MarkerID, MarkerWidget);
    }
}

void UPGMinimapWidget::UpdateMarkerPosition(FName MarkerID, const FVector2D& Position)
{
    // 마커 위젯 찾기
    if (!MarkerWidgets.Contains(MarkerID))
    {
        return;
    }
    
    UPGMinimapMarkerWidget* MarkerWidget = MarkerWidgets[MarkerID];
    if (!MarkerWidget)
    {
        return;
    }
    
    // 위치 업데이트
    UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(MarkerWidget->Slot);
    if (MarkerSlot)
    {
        MarkerSlot->SetPosition(Position);
    }
}

void UPGMinimapWidget::RemoveMarker(FName MarkerID)
{
    // 마커 위젯 찾기
    if (!MarkerWidgets.Contains(MarkerID))
    {
        return;
    }
    
    UPGMinimapMarkerWidget* MarkerWidget = MarkerWidgets[MarkerID];
    if (MarkerWidget)
    {
        // UI에서 제거
        MarkerWidget->RemoveFromParent();
        
        // 맵에서 제거
        MarkerWidgets.Remove(MarkerID);
    }
}

void UPGMinimapWidget::ClearAllMarkers()
{
    // 모든 마커 제거
    for (auto& Pair : MarkerWidgets)
    {
        if (Pair.Value)
        {
            Pair.Value->RemoveFromParent();
        }
    }
    
    // 맵 비우기
    MarkerWidgets.Empty();
}

void UPGMinimapWidget::OnZoomInClicked()
{
    SetMinimapZoom(CurrentZoom + ZoomStep);
}

void UPGMinimapWidget::OnZoomOutClicked()
{
    SetMinimapZoom(CurrentZoom - ZoomStep);
}