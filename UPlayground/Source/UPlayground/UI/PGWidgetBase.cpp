// PGWidgetBase.cpp
#include "PGWidgetBase.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"

UPGWidgetBase::UPGWidgetBase(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CurrentState(EPGWidgetState::Inactive)
    , ZOrder(0)
    , bCloseOnEsc(true)
{
}

void UPGWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 초기화 함수 호출
    InitializeWidget();
}

void UPGWidgetBase::NativeDestruct()
{
    Super::NativeDestruct();
}

void UPGWidgetBase::InitializeWidget()
{
    // 블루프린트에서 확장 가능한 초기화 함수
}

void UPGWidgetBase::ShowWidget()
{
    if (CurrentState == EPGWidgetState::Inactive || CurrentState == EPGWidgetState::Disappearing)
    {
        AddToViewport(ZOrder);
        CurrentState = EPGWidgetState::Appearing;
        PlayShowAnimation();
    }
}

void UPGWidgetBase::HideWidget()
{
    if (CurrentState == EPGWidgetState::Active || CurrentState == EPGWidgetState::Appearing)
    {
        CurrentState = EPGWidgetState::Disappearing;
        PlayHideAnimation();
    }
}

void UPGWidgetBase::DestroyWidget()
{
    RemoveFromParent();
}

void UPGWidgetBase::PlayShowAnimation()
{
    if (ShowAnimation)
    {
        // 애니메이션 재생 및 완료 이벤트 바인딩
        PlayAnimation(ShowAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
        
        // 애니메이션 완료 시 호출될 델리게이트 설정
        FWidgetAnimationDynamicEvent AnimationFinishedEvent;
        AnimationFinishedEvent.BindDynamic(this, &UPGWidgetBase::OnShowAnimationFinished);
        BindToAnimationFinished(ShowAnimation, AnimationFinishedEvent);
    }
    else
    {
        // 애니메이션이 없으면 즉시 완료 처리
        OnShowAnimationFinished();
    }
}

void UPGWidgetBase::PlayHideAnimation()
{
    if (HideAnimation)
    {
        // 애니메이션 재생 및 완료 이벤트 바인딩
        PlayAnimation(HideAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f);
        
        // 애니메이션 완료 시 호출될 델리게이트 설정
        FWidgetAnimationDynamicEvent AnimationFinishedEvent;
        AnimationFinishedEvent.BindDynamic(this, &UPGWidgetBase::OnHideAnimationFinished);
        BindToAnimationFinished(HideAnimation, AnimationFinishedEvent);
    }
    else
    {
        // 애니메이션이 없으면 즉시 완료 처리
        OnHideAnimationFinished();
    }
}

void UPGWidgetBase::OnShowAnimationFinished()
{
    CurrentState = EPGWidgetState::Active;
}

void UPGWidgetBase::OnHideAnimationFinished()
{
    CurrentState = EPGWidgetState::Inactive;
    DestroyWidget();
}

FReply UPGWidgetBase::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    // ESC 키로 위젯 닫기
    if (bCloseOnEsc && InKeyEvent.GetKey() == EKeys::Escape && IsWidgetActive())
    {
        HideWidget();
        return FReply::Handled();
    }
    
    return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}