#include "UI/Widgets/Elements/PGQuickSlotItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Button.h"

UPGQuickSlotItemWidget::UPGQuickSlotItemWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGQuickSlotItemWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 버튼 클릭 이벤트 바인딩
    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UPGQuickSlotItemWidget::OnButtonClicked);
    }
}

void UPGQuickSlotItemWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // 쿨다운 업데이트
    if (CurrentItemData.RemainingCooldown > 0)
    {
        CurrentItemData.RemainingCooldown = FMath::Max(0.0f, CurrentItemData.RemainingCooldown - InDeltaTime);
        UpdateCooldown(CurrentItemData.RemainingCooldown);
    }
}

void UPGQuickSlotItemWidget::UpdateSlotData(const FPGQuickSlotItemData& ItemData)
{
    CurrentItemData = ItemData;
    
    // 아이콘 업데이트
    if (ItemIcon)
    {
        if (ItemData.ItemType != EPGQuickSlotItemType::None && ItemData.ItemIcon)
        {
            ItemIcon->SetBrushFromTexture(ItemData.ItemIcon);
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    // 수량 업데이트
    UpdateQuantity(ItemData.Quantity);
    
    // 쿨다운 업데이트
    UpdateCooldown(ItemData.RemainingCooldown);
    
    // 툴팁 설정
    if (ItemData.ItemType != EPGQuickSlotItemType::None)
    {
        SetToolTipText(ItemData.ItemName);
    }
    else
    {
        SetToolTipText(FText::GetEmpty());
    }
}

void UPGQuickSlotItemWidget::UpdateCooldown(float RemainingCooldown)
{
    CurrentItemData.RemainingCooldown = RemainingCooldown;
    
    if (CurrentItemData.Cooldown <= 0)
    {
        // 쿨다운이 없는 아이템
        if (CooldownOverlay)
        {
            CooldownOverlay->SetVisibility(ESlateVisibility::Collapsed);
        }
        
        if (CooldownText)
        {
            CooldownText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    else
    {
        // 쿨다운 진행 상태 계산
        const float CooldownRatio = RemainingCooldown / CurrentItemData.Cooldown;
        
        // 오버레이 업데이트
        if (CooldownOverlay)
        {
            if (RemainingCooldown > 0)
            {
                CooldownOverlay->SetPercent(CooldownRatio);
                CooldownOverlay->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                CooldownOverlay->SetVisibility(ESlateVisibility::Collapsed);
            }
        }
        
        // 쿨다운 텍스트 업데이트
        if (CooldownText)
        {
            if (RemainingCooldown > 0)
            {
                CooldownText->SetText(FText::AsNumber(FMath::CeilToInt(RemainingCooldown)));
                CooldownText->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                CooldownText->SetVisibility(ESlateVisibility::Collapsed);
            }
        }
    }
}

void UPGQuickSlotItemWidget::UpdateQuantity(int32 Quantity)
{
    CurrentItemData.Quantity = Quantity;
    
    if (QuantityText)
    {
        if (Quantity > 1)
        {
            QuantityText->SetText(FText::AsNumber(Quantity));
            QuantityText->SetVisibility(ESlateVisibility::Visible);
        }
        else
        {
            QuantityText->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

void UPGQuickSlotItemWidget::SetSlotIndex(int32 Index)
{
    SlotIndex = Index;
}

void UPGQuickSlotItemWidget::SetKeyBindingText(const FText& KeyText)
{
    if (KeyBindingText)
    {
        KeyBindingText->SetText(KeyText);
    }
}

void UPGQuickSlotItemWidget::OnButtonClicked()
{
    OnSlotClicked.Broadcast(SlotIndex);
}