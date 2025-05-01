#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Base/PGHUDBaseWidget.h"
#include "PGQuestWidget.generated.h"

UCLASS()
class UPLAYGROUND_API UPGQuestWidget : public UPGHUDBaseWidget
{
    GENERATED_BODY()
    
public:
    UPGQuestWidget(const FObjectInitializer& ObjectInitializer);
    
    // 위젯 업데이트 오버라이드
    virtual void UpdateWidget() override;
    
    // 퀘스트 표시/숨김
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void ToggleQuest();
    
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void ShowQuest();
    
    UFUNCTION(BlueprintCallable, Category = "Quest")
    void HideQuest();
    
    // 상태 확인
    UFUNCTION(BlueprintPure, Category = "Quest")
    bool IsQuestOpen() const { return bIsQuestOpen; }
    
protected:
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> QuestPanel;
    
    // 퀘스트 상태
    UPROPERTY(BlueprintReadOnly, Category = "Quest")
    bool bIsQuestOpen = false;
    
    // 애니메이션
    UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
    UWidgetAnimation* OpenAnimation;
    
    UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
    UWidgetAnimation* CloseAnimation;
    
    // 애니메이션 종료 콜백
    UFUNCTION()
    void OnCloseAnimationFinished();
    
    // 초기 설정
    virtual void NativeConstruct() override;
};