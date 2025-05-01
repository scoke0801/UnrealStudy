#include "UI/Widgets/Elements/PGMinimapMarkerWidget.h"
#include "Components/Image.h"

UPGMinimapMarkerWidget::UPGMinimapMarkerWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGMinimapMarkerWidget::SetMarkerData(FName InMarkerID, UTexture2D* MarkerIcon, const FLinearColor& InMarkerColor)
{
    MarkerID = InMarkerID;
    MarkerColor = InMarkerColor;
    
    // 아이콘 및 색상 설정
    if (MarkerImage)
    {
        if (MarkerIcon)
        {
            MarkerImage->SetBrushFromTexture(MarkerIcon);
        }
        
        MarkerImage->SetColorAndOpacity(MarkerColor);
    }
}