#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Base/PGHUDBaseWidget.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGQuickSlotWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuickSlotItemUsed, int32, SlotIndex);

UCLASS()
class UPLAYGROUND_API UPGQuickSlotWidget : public UPGHUDBaseWidget
{
    GENERATED_BODY()
    
public:
    UPGQuickSlotWidget(const FObjectInitializer& ObjectInitializer);
    
    // 위젯 업데이트 오버라이드
    virtual void UpdateWidget() override;
    
    // 슬롯 관리 함수
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void SetQuickSlotItem(int32 SlotIndex, const FPGQuickSlotItemData& ItemData);
    
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void ClearQuickSlot(int32 SlotIndex);
    
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UpdateCooldown(int32 SlotIndex, float RemainingCooldown);
    
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UpdateQuantity(int32 SlotIndex, int32 Quantity);
    
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void ActivateSlot(int32 SlotIndex);
    
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    FPGQuickSlotItemData GetSlotData(int32 SlotIndex) const;
    
    // 키 바인딩 업데이트
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void SetKeyBindingText(int32 SlotIndex, const FText& KeyBindingText);
    
public:
    // 이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "QuickSlot|Events")
    FOnQuickSlotItemUsed OnQuickSlotItemUsed;
    
protected:
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UHorizontalBox> QuickSlotContainer;
    
    // 퀵슬롯 항목 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "QuickSlot")
    TSubclassOf<class UPGQuickSlotItemWidget> QuickSlotItemWidgetClass;
    
    // 퀵슬롯 개수
    UPROPERTY(EditDefaultsOnly, Category = "QuickSlot")
    int32 QuickSlotCount = 10;
    
    // 슬롯 데이터
    UPROPERTY(BlueprintReadOnly, Category = "QuickSlot")
    TArray<FPGQuickSlotItemData> SlotItems;
    
    // 슬롯 위젯
    UPROPERTY()
    TArray<TObjectPtr<class UPGQuickSlotItemWidget>> SlotWidgets;
    
    // EnhancedInput 처리
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void HandleInputAction(int32 SlotIndex);
    
private:
    // 내부 함수
    void InitializeQuickSlots();
};