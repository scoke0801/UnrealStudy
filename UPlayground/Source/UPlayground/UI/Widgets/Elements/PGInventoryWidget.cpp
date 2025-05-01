#include "UI/Widgets/Elements/PGInventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Animation/WidgetAnimation.h"

UPGInventoryWidget::UPGInventoryWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 초기 상태 설정
    if (InventoryPanel)
    {
        InventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    bIsInventoryOpen = false;
}

void UPGInventoryWidget::UpdateWidget()
{
    Super::UpdateWidget();
    
    // 인벤토리 초기화 (필요한 경우)
}

void UPGInventoryWidget::ToggleInventory()
{
    if (bIsInventoryOpen)
    {
        HideInventory();
    }
    else
    {
        ShowInventory();
    }
}

void UPGInventoryWidget::ShowInventory()
{
    if (bIsInventoryOpen)
    {
        return;
    }
    
    bIsInventoryOpen = true;
    
    if (InventoryPanel)
    {
        InventoryPanel->SetVisibility(ESlateVisibility::Visible);
    }
    
    // 애니메이션 재생
    if (OpenAnimation)
    {
        PlayAnimation(OpenAnimation);
    }
}

void UPGInventoryWidget::HideInventory()
{
    if (!bIsInventoryOpen)
    {
        return;
    }
    
    bIsInventoryOpen = false;
    
    // 애니메이션 재생
    if (CloseAnimation)
    {
        PlayAnimation(CloseAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
        
        // 애니메이션 완료 후 패널 숨기기 (FTimerHandle 사용)
        float AnimDuration = CloseAnimation->GetEndTime();
        
        // 타이머 설정 - FTimerManager의 적절한 오버로드 사용
        if (GetWorld())
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                FTimerDelegate::CreateUObject(this, &UPGInventoryWidget::OnCloseAnimationFinished),
                AnimDuration,
                false
            );
        }
        else
        {
            // World가 없는 경우 바로 숨기기
            OnCloseAnimationFinished();
        }
    }
    else
    {
        // 애니메이션 없으면 바로 숨기기
        OnCloseAnimationFinished();
    }
}

// 애니메이션 종료 후 호출될 함수
void UPGInventoryWidget::OnCloseAnimationFinished()
{
    if (InventoryPanel)
    {
        InventoryPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
}