#include "UI/Widgets/Elements/PGQuickSlotWidget.h"
#include "UI/Widgets/Elements/PGQuickSlotItemWidget.h"
#include "Components/HorizontalBox.h"

UPGQuickSlotWidget::UPGQuickSlotWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 기본 설정
}

void UPGQuickSlotWidget::UpdateWidget()
{
    Super::UpdateWidget();
    
    // 퀵슬롯 초기화
    InitializeQuickSlots();
}

void UPGQuickSlotWidget::InitializeQuickSlots()
{
    // 데이터 배열 초기화
    SlotItems.SetNum(QuickSlotCount);
    
    // 위젯 컨테이너가 없으면 반환
    if (!QuickSlotContainer || !QuickSlotItemWidgetClass)
    {
        return;
    }
    
    // 기존 슬롯 제거
    QuickSlotContainer->ClearChildren();
    SlotWidgets.Empty();
    
    // 새 슬롯 생성
    for (int32 i = 0; i < QuickSlotCount; ++i)
    {
        UPGQuickSlotItemWidget* SlotWidget = CreateWidget<UPGQuickSlotItemWidget>(this, QuickSlotItemWidgetClass);
        if (SlotWidget)
        {
            SlotWidget->SetSlotIndex(i);
            
            // 키 바인딩 텍스트 설정 (1~9, 0)
            FText KeyText;
            if (i < 9)
            {
                KeyText = FText::AsNumber(i + 1);
            }
            else if (i == 9)
            {
                KeyText = FText::FromString(TEXT("0"));
            }
            SlotWidget->SetKeyBindingText(KeyText);
            
            // 버튼 클릭 이벤트 바인딩
            SlotWidget->OnSlotClicked.AddDynamic(this, &UPGQuickSlotWidget::HandleInputAction);
            
            // 빈 데이터로 초기화
            FPGQuickSlotItemData EmptyData;
            EmptyData.ItemType = EPGQuickSlotItemType::None;
            SlotWidget->UpdateSlotData(EmptyData);
            
            // 컨테이너에 추가
            QuickSlotContainer->AddChild(SlotWidget);
            SlotWidgets.Add(SlotWidget);
        }
    }
}

void UPGQuickSlotWidget::SetQuickSlotItem(int32 SlotIndex, const FPGQuickSlotItemData& ItemData)
{
    // 인덱스 유효성 검사
    if (SlotIndex < 0 || SlotIndex >= SlotItems.Num())
    {
        return;
    }
    
    // 데이터 업데이트
    SlotItems[SlotIndex] = ItemData;
    
    // 위젯 업데이트
    if (SlotIndex < SlotWidgets.Num() && SlotWidgets[SlotIndex])
    {
        SlotWidgets[SlotIndex]->UpdateSlotData(ItemData);
    }
}

void UPGQuickSlotWidget::ClearQuickSlot(int32 SlotIndex)
{
    // 인덱스 유효성 검사
    if (SlotIndex < 0 || SlotIndex >= SlotItems.Num())
    {
        return;
    }
    
    // 빈 슬롯 데이터 생성
    FPGQuickSlotItemData EmptyData;
    EmptyData.ItemType = EPGQuickSlotItemType::None;
    
    // 데이터 업데이트
    SlotItems[SlotIndex] = EmptyData;
    
    // 위젯 업데이트
    if (SlotIndex < SlotWidgets.Num() && SlotWidgets[SlotIndex])
    {
        SlotWidgets[SlotIndex]->UpdateSlotData(EmptyData);
    }
}

void UPGQuickSlotWidget::UpdateCooldown(int32 SlotIndex, float RemainingCooldown)
{
    // 인덱스 유효성 검사
    if (SlotIndex < 0 || SlotIndex >= SlotItems.Num())
    {
        return;
    }
    
    // 데이터 업데이트
    SlotItems[SlotIndex].RemainingCooldown = RemainingCooldown;
    
    // 위젯 업데이트
    if (SlotIndex < SlotWidgets.Num() && SlotWidgets[SlotIndex])
    {
        SlotWidgets[SlotIndex]->UpdateCooldown(RemainingCooldown);
    }
}

void UPGQuickSlotWidget::UpdateQuantity(int32 SlotIndex, int32 Quantity)
{
    // 인덱스 유효성 검사
    if (SlotIndex < 0 || SlotIndex >= SlotItems.Num())
    {
        return;
    }
    
    // 데이터 업데이트
    SlotItems[SlotIndex].Quantity = Quantity;
    
    // 위젯 업데이트
    if (SlotIndex < SlotWidgets.Num() && SlotWidgets[SlotIndex])
    {
        SlotWidgets[SlotIndex]->UpdateQuantity(Quantity);
    }
}

void UPGQuickSlotWidget::ActivateSlot(int32 SlotIndex)
{
    // 인덱스 유효성 검사
    if (SlotIndex < 0 || SlotIndex >= SlotItems.Num())
    {
        return;
    }
    
    // 슬롯 데이터 가져오기
    const FPGQuickSlotItemData& SlotData = SlotItems[SlotIndex];
    
    // 항목이 없거나 사용할 수 없는 경우 무시
    if (SlotData.ItemType == EPGQuickSlotItemType::None || !SlotData.bIsAvailable)
    {
        return;
    }
    
    // 쿨다운 중인지 확인
    if (SlotData.RemainingCooldown > 0)
    {
        return;
    }
    
    // 이벤트 발송
    OnQuickSlotItemUsed.Broadcast(SlotIndex);
}

FPGQuickSlotItemData UPGQuickSlotWidget::GetSlotData(int32 SlotIndex) const
{
    // 인덱스 유효성 검사
    if (SlotIndex < 0 || SlotIndex >= SlotItems.Num())
    {
        FPGQuickSlotItemData EmptyData;
        EmptyData.ItemType = EPGQuickSlotItemType::None;
        return EmptyData;
    }
    
    return SlotItems[SlotIndex];
}

void UPGQuickSlotWidget::SetKeyBindingText(int32 SlotIndex, const FText& KeyBindingText)
{
    // 인덱스 유효성 검사
    if (SlotIndex < 0 || SlotIndex >= SlotWidgets.Num())
    {
        return;
    }
    
    // 위젯 업데이트
    if (SlotWidgets[SlotIndex])
    {
        SlotWidgets[SlotIndex]->SetKeyBindingText(KeyBindingText);
    }
}

void UPGQuickSlotWidget::HandleInputAction(int32 SlotIndex)
{
    ActivateSlot(SlotIndex);
}