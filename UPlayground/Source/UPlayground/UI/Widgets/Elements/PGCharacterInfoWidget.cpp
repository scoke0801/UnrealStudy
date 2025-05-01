#include "UI/Widgets/Elements/PGCharacterInfoWidget.h"
#include "UI/Widgets/Elements/PGBuffWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"

UPGCharacterInfoWidget::UPGCharacterInfoWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGCharacterInfoWidget::UpdateWidget()
{
    Super::UpdateWidget();
    
    // 초기 데이터로 위젯 설정
    UpdateHealthBar(100.0f, 100.0f);
    UpdateExperienceBar(0.0f, 1000.0f, 1);
    
    // 캐릭터 이름 설정 (예시)
    if (CharacterNameText)
    {
        CharacterNameText->SetText(FText::FromString(TEXT("Player")));
    }
}

void UPGCharacterInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // 버프 타이머 업데이트
    TArray<FName> BuffsToRemove;
    
    for (auto& BuffPair : ActiveBuffs)
    {
        FName BuffID = BuffPair.Key;
        FPGBuffData& BuffData = BuffPair.Value;
        
        if (BuffData.Duration > 0)
        {
            BuffData.RemainingTime -= InDeltaTime;
            
            if (BuffData.RemainingTime <= 0)
            {
                BuffsToRemove.Add(BuffID);
            }
            else
            {
                UpdateBuffDuration(BuffID, BuffData.RemainingTime);
            }
        }
    }
    
    // 만료된 버프 제거
    for (const FName& BuffID : BuffsToRemove)
    {
        RemoveBuff(BuffID);
    }
}

void UPGCharacterInfoWidget::UpdateCharacterImage(UTexture2D* InCharacterImage)
{
    if (CharacterImage && InCharacterImage)
    {
        CharacterImage->SetBrushFromTexture(InCharacterImage);
    }
}

void UPGCharacterInfoWidget::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
    if (HealthBar)
    {
        HealthBar->SetPercent(MaxHealth > 0 ? (CurrentHealth / MaxHealth) : 0.0f);
    }
    
    if (HealthText)
    {
        HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f/%.0f"), CurrentHealth, MaxHealth)));
    }
}

void UPGCharacterInfoWidget::UpdateExperienceBar(float CurrentExp, float MaxExp, int32 Level)
{
    if (ExperienceBar)
    {
        ExperienceBar->SetPercent(MaxExp > 0 ? (CurrentExp / MaxExp) : 0.0f);
    }
    
    if (LevelText)
    {
        LevelText->SetText(FText::FromString(FString::Printf(TEXT("LV. %d"), Level)));
    }
}

void UPGCharacterInfoWidget::AddBuff(const FPGBuffData& BuffData)
{
    // 이미 존재하는 버프인지 확인
    if (ActiveBuffs.Contains(BuffData.BuffID))
    {
        // 기존 버프 업데이트 (스택 또는 시간 갱신)
        FPGBuffData& ExistingBuff = ActiveBuffs[BuffData.BuffID];
        ExistingBuff.StackCount = BuffData.StackCount > 0 ? BuffData.StackCount : ExistingBuff.StackCount + 1;
        ExistingBuff.RemainingTime = BuffData.Duration > 0 ? BuffData.RemainingTime : ExistingBuff.RemainingTime;
        
        // 위젯이 있다면 업데이트
        if (BuffWidgets.Contains(BuffData.BuffID))
        {
            BuffWidgets[BuffData.BuffID]->UpdateBuffData(ExistingBuff);
        }
    }
    else
    {
        // 새 버프 추가
        ActiveBuffs.Add(BuffData.BuffID, BuffData);
        
        // 위젯 생성
        if (BuffWidgetClass && BuffContainer)
        {
            UPGBuffWidget* NewBuffWidget = CreateWidget<UPGBuffWidget>(this, BuffWidgetClass);
            if (NewBuffWidget)
            {
                NewBuffWidget->UpdateBuffData(BuffData);
                BuffContainer->AddChild(NewBuffWidget);
                BuffWidgets.Add(BuffData.BuffID, NewBuffWidget);
            }
        }
    }
}

void UPGCharacterInfoWidget::RemoveBuff(FName BuffID)
{
    // 버프 데이터 제거
    ActiveBuffs.Remove(BuffID);
    
    // 위젯 제거
    if (BuffWidgets.Contains(BuffID))
    {
        BuffWidgets[BuffID]->RemoveFromParent();
        BuffWidgets.Remove(BuffID);
    }
}

void UPGCharacterInfoWidget::UpdateBuffDuration(FName BuffID, float RemainingTime)
{
    if (ActiveBuffs.Contains(BuffID))
    {
        ActiveBuffs[BuffID].RemainingTime = RemainingTime;
        
        if (BuffWidgets.Contains(BuffID))
        {
            BuffWidgets[BuffID]->UpdateBuffDuration(RemainingTime);
        }
    }
}

void UPGCharacterInfoWidget::UpdateBuffStack(FName BuffID, int32 StackCount)
{
    if (ActiveBuffs.Contains(BuffID))
    {
        ActiveBuffs[BuffID].StackCount = StackCount;
        
        if (BuffWidgets.Contains(BuffID))
        {
            BuffWidgets[BuffID]->UpdateBuffStack(StackCount);
        }
    }
}

void UPGCharacterInfoWidget::ClearAllBuffs()
{
    ActiveBuffs.Empty();
    
    for (auto& WidgetPair : BuffWidgets)
    {
        if (WidgetPair.Value)
        {
            WidgetPair.Value->RemoveFromParent();
        }
    }
    
    BuffWidgets.Empty();
}

void UPGCharacterInfoWidget::UpdateBuffsDisplay()
{
    // 모든 버프 UI 갱신 (필요시 사용)
    for (auto& BuffPair : ActiveBuffs)
    {
        const FName& BuffID = BuffPair.Key;
        const FPGBuffData& BuffData = BuffPair.Value;
        
        if (BuffWidgets.Contains(BuffID))
        {
            BuffWidgets[BuffID]->UpdateBuffData(BuffData);
        }
    }
}