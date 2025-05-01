#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGHUDContainerWidget.generated.h"

UCLASS()
class UPLAYGROUND_API UPGHUDContainerWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPGHUDContainerWidget(const FObjectInitializer& ObjectInitializer);
    
    // HUD 요소 추가
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void AddHUDElement(class UPGHUDBaseWidget* Widget, EPGHUDLayer Layer);
    
    // HUD 레이어 패널 가져오기
    UFUNCTION(BlueprintPure, Category = "HUD")
    class UPanelWidget* GetLayerPanel(EPGHUDLayer Layer) const;
    
    // 코너 패널 얻기
    UFUNCTION(BlueprintPure, Category = "HUD")
    class UPanelWidget* GetCornerPanel(EPGHUDCornerPosition Position) const;
    
    // 코너에 요소 추가
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void AddElementToCorner(class UPGHUDBaseWidget* Widget, EPGHUDCornerPosition Position);
    
protected:
    // 레이어 패널 참조
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> BackgroundLayerPanel;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> GameLayerPanel;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> ForegroundLayerPanel;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> ModalLayerPanel;
    
    // 코너 패널 참조
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> TopLeftPanel;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> TopRightPanel;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> BottomLeftPanel;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> BottomRightPanel;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> CenterPanel;
    
    // NativeConstruct 오버라이드
    virtual void NativeConstruct() override;
};