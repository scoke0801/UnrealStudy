#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Base/PGHUDBaseWidget.h"
#include "UI/Widgets/Elements/PGQuickSlotItemWidget.h"
#include "PGInventoryWidget.generated.h"

/**
 * 인벤토리 UI 위젯
 */
UCLASS()
class UPLAYGROUND_API UPGInventoryWidget : public UPGHUDBaseWidget
{
    GENERATED_BODY()
    
public:
    UPGInventoryWidget(const FObjectInitializer& ObjectInitializer);
    
    // 위젯 업데이트 오버라이드
    virtual void UpdateWidget() override;
    
    // 인벤토리 표시/숨김
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ToggleInventory();
    
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void ShowInventory();
    
    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void HideInventory();
    
    // 상태 확인
    UFUNCTION(BlueprintPure, Category = "Inventory")
    bool IsInventoryOpen() const { return bIsInventoryOpen; }
    
protected:
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> InventoryPanel;
    
    // 인벤토리 상태
    UPROPERTY(BlueprintReadOnly, Category = "Inventory")
    bool bIsInventoryOpen = false;
    
    // 애니메이션
    UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
    UWidgetAnimation* OpenAnimation;
    
    UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
    UWidgetAnimation* CloseAnimation;
    
    // 초기 설정
    virtual void NativeConstruct() override;
    
    // 애니메이션 종료 콜백
    UFUNCTION()
    void OnCloseAnimationFinished();
};