#include "UI/Widgets/Elements/PGQuestWidget.h"
#include "Components/CanvasPanel.h"
#include "Animation/WidgetAnimation.h"

UPGQuestWidget::UPGQuestWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGQuestWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 초기 상태 설정
    if (QuestPanel)
    {
        QuestPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    bIsQuestOpen = false;
}

void UPGQuestWidget::UpdateWidget()
{
    Super::UpdateWidget();
    
    // 퀘스트 초기화 (필요한 경우)
}

void UPGQuestWidget::ToggleQuest()
{
    if (bIsQuestOpen)
    {
        HideQuest();
    }
    else
    {
        ShowQuest();
    }
}

void UPGQuestWidget::ShowQuest()
{
    if (bIsQuestOpen)
    {
        return;
    }
    
    bIsQuestOpen = true;
    
    if (QuestPanel)
    {
        QuestPanel->SetVisibility(ESlateVisibility::Visible);
    }
    
    // 애니메이션 재생
    if (OpenAnimation)
    {
        PlayAnimation(OpenAnimation);
    }
}

void UPGQuestWidget::HideQuest()
{
    if (!bIsQuestOpen)
    {
        return;
    }
    
    bIsQuestOpen = false;
    
    // 애니메이션 재생
    if (CloseAnimation)
    {
        PlayAnimation(CloseAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
        
        // 애니메이션 종료 후 패널 숨기기
        float AnimDuration = CloseAnimation->GetEndTime();
        
        // 타이머 설정 - FTimerManager의 적절한 오버로드 사용
        if (GetWorld())
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                FTimerDelegate::CreateUObject(this, &UPGQuestWidget::OnCloseAnimationFinished),
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

void UPGQuestWidget::OnCloseAnimationFinished()
{
    if (QuestPanel)
    {
        QuestPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
}