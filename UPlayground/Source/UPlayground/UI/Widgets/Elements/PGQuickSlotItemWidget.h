#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGQuickSlotItemWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClickedEvent, int32, SlotIndex);

UCLASS()
class UPLAYGROUND_API UPGQuickSlotItemWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPGQuickSlotItemWidget(const FObjectInitializer& ObjectInitializer);
    
    // 슬롯 데이터 업데이트
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UpdateSlotData(const FPGQuickSlotItemData& ItemData);
    
    // 쿨다운 업데이트
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UpdateCooldown(float RemainingCooldown);
    
    // 수량 업데이트
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void UpdateQuantity(int32 Quantity);
    
    // 슬롯 인덱스 설정
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void SetSlotIndex(int32 Index);
    
    // 키 바인딩 텍스트 설정
    UFUNCTION(BlueprintCallable, Category = "QuickSlot")
    void SetKeyBindingText(const FText& KeyText);
    
    // 현재 슬롯 인덱스 가져오기
    UFUNCTION(BlueprintPure, Category = "QuickSlot")
    int32 GetSlotIndex() const { return SlotIndex; }
    
public:
    // 이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "QuickSlot|Events")
    FOnSlotClickedEvent OnSlotClicked;
    
protected:
    // 틱 업데이트
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UImage> ItemIcon;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> QuantityText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> CooldownText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UProgressBar> CooldownOverlay;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> KeyBindingText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UButton> SlotButton;
    
    // 슬롯 데이터
    UPROPERTY(BlueprintReadOnly, Category = "QuickSlot")
    FPGQuickSlotItemData CurrentItemData;
    
    // 슬롯 인덱스
    UPROPERTY(BlueprintReadOnly, Category = "QuickSlot")
    int32 SlotIndex = -1;
    
    // 버튼 클릭 이벤트 핸들러
    UFUNCTION()
    void OnButtonClicked();
    
    // 초기 설정
    virtual void NativeConstruct() override;
};