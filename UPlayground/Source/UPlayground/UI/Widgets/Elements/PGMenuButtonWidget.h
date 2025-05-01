#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGMenuButtonWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuButtonClickedEvent, FName, MenuID);

UCLASS()
class UPLAYGROUND_API UPGMenuButtonWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPGMenuButtonWidget(const FObjectInitializer& ObjectInitializer);
    
    // 버튼 데이터 설정
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void SetButtonData(const FPGMenuItemData& MenuItemData);
    
    // 메뉴 ID 가져오기
    UFUNCTION(BlueprintPure, Category = "Menu")
    FName GetMenuID() const { return MenuID; }
    
public:
    // 이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Menu|Events")
    FOnMenuButtonClickedEvent OnMenuButtonClicked;
    
protected:
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UButton> MenuButton;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UImage> MenuIcon;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> MenuText;
    
    // 메뉴 데이터
    UPROPERTY(BlueprintReadOnly, Category = "Menu")
    FName MenuID;
    
    UPROPERTY(BlueprintReadOnly, Category = "Menu")
    FName ActionName;
    
    // 버튼 클릭 이벤트 핸들러
    UFUNCTION()
    void OnButtonClicked();
    
    // 초기 설정
    virtual void NativeConstruct() override;
};