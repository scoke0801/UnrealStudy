#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Base/PGHUDBaseWidget.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGMenuWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuItemSelected, FName, MenuID);

UCLASS()
class UPLAYGROUND_API UPGMenuWidget : public UPGHUDBaseWidget
{
    GENERATED_BODY()
    
public:
    UPGMenuWidget(const FObjectInitializer& ObjectInitializer);
    
    // 위젯 업데이트 오버라이드
    virtual void UpdateWidget() override;
    
    // 메뉴 버튼 설정
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void SetMenuItems(const TArray<FPGMenuItemData>& MenuItems);
    
    // 메뉴 열기/닫기
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ToggleMenu();
    
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OpenMenu();
    
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void CloseMenu();
    
    // 상태 확인
    UFUNCTION(BlueprintPure, Category = "Menu")
    bool IsMenuOpen() const { return bIsMenuOpen; }
    
public:
    // 이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Menu|Events")
    FOnMenuItemSelected OnMenuItemSelected;
    
protected:
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UVerticalBox> MenuButtonsContainer;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UButton> MenuToggleButton;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UCanvasPanel> MenuPanel;
    
    // 메뉴 버튼 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "Menu")
    TSubclassOf<class UPGMenuButtonWidget> MenuButtonWidgetClass;
    
    // 메뉴 상태
    UPROPERTY(BlueprintReadOnly, Category = "Menu")
    bool bIsMenuOpen = false;
    
    // 현재 메뉴 항목
    UPROPERTY(BlueprintReadOnly, Category = "Menu")
    TArray<FPGMenuItemData> CurrentMenuItems;
    
    // 애니메이션
    UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
    UWidgetAnimation* MenuOpenAnimation;
    
    UPROPERTY(Transient, BlueprintReadOnly, meta = (BindWidgetAnim))
    UWidgetAnimation* MenuCloseAnimation;
    
    // 이벤트 핸들러
    UFUNCTION()
    void OnMenuButtonClicked(FName MenuID);
    
    UFUNCTION()
    void OnToggleButtonClicked();
    
    // 애니메이션 종료 콜백
    UFUNCTION()
    void OnCloseAnimationFinished();
    
    // 초기 설정
    virtual void NativeConstruct() override;
    
private:
    // 내부 함수
    void CreateMenuButtons();
};