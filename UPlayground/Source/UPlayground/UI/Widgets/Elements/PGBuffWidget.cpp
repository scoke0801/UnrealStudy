#include "UI/Widgets/Elements/PGBuffWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

UPGBuffWidget::UPGBuffWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGBuffWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // 필요한 경우 버프 상태 업데이트 (시각적 효과 등)
}

void UPGBuffWidget::UpdateBuffData(const FPGBuffData& BuffData)
{
    CurrentBuffData = BuffData;
    
    // 아이콘 업데이트
    if (BuffIcon && BuffData.BuffIcon)
    {
        BuffIcon->SetBrushFromTexture(BuffData.BuffIcon);
        
        // 버프 타입에 따른 색상 설정
        FLinearColor IconColor = FLinearColor::White;
        switch (BuffData.BuffType)
        {
            case EPGBuffType::Positive:
                IconColor = FLinearColor(0.2f, 0.8f, 0.2f); // 녹색
                break;
            case EPGBuffType::Negative:
                IconColor = FLinearColor(0.8f, 0.2f, 0.2f); // 빨간색
                break;
            case EPGBuffType::Neutral:
            default:
                IconColor = FLinearColor(0.5f, 0.5f, 0.8f); // 파란색
                break;
        }
        
        BuffIcon->SetColorAndOpacity(IconColor);
    }
    
    // 스택 텍스트 업데이트
    if (StackText)
    {
        if (BuffData.StackCount > 1)
        {
            StackText->SetText(FText::AsNumber(BuffData.StackCount));
            StackText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            StackText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    // 지속시간 관련 UI 업데이트
    UpdateBuffDuration(BuffData.RemainingTime);
    
    // 툴팁 설정
    SetBuffTooltip();
}

void UPGBuffWidget::UpdateBuffDuration(float RemainingTime)
{
    CurrentBuffData.RemainingTime = RemainingTime;
    
    if (CurrentBuffData.Duration <= 0) // 영구 버프
    {
        if (TimeRemainingText)
        {
            TimeRemainingText->SetVisibility(ESlateVisibility::Collapsed);
        }
        
        if (DurationBar)
        {
            DurationBar->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    else // 시간 제한 버프
    {
        if (TimeRemainingText)
        {
            TimeRemainingText->SetText(FText::AsNumber(FMath::CeilToInt(RemainingTime)));
            TimeRemainingText->SetVisibility(ESlateVisibility::Visible);
        }
        
        if (DurationBar)
        {
            DurationBar->SetPercent(RemainingTime / CurrentBuffData.Duration);
            DurationBar->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void UPGBuffWidget::UpdateBuffStack(int32 StackCount)
{
    CurrentBuffData.StackCount = StackCount;
    
    if (StackText)
    {
        if (StackCount > 1)
        {
            StackText->SetText(FText::AsNumber(StackCount));
            StackText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            StackText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    // 툴팁 업데이트
    SetBuffTooltip();
}

void UPGBuffWidget::SetBuffTooltip()
{
    FText TooltipText = FText::Format(
        NSLOCTEXT("BuffWidget", "BuffTooltip", "{0}\n{1}"),
        CurrentBuffData.BuffName,
        CurrentBuffData.BuffDescription
    );
    
    SetToolTipText(TooltipText);
}