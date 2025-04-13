// PGPopup.h
#pragma once

#include "CoreMinimal.h"
#include "PGWidgetBase.h"
#include "PGPopup.generated.h"

UENUM(BlueprintType)
enum class EPGPopupType : uint8
{
    Default,    // 기본 팝업
    Info,       // 정보 팝업
    Warning,    // 경고 팝업
    Error,      // 오류 팝업
    Success     // 성공 팝업
};

UENUM(BlueprintType)
enum class EPGPopupMode : uint8
{
    Modal,      // 모달 팝업 (배경 입력 차단)
    Modaless    // 모달리스 팝업 (배경 입력 허용)
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPopupButtonClickedDelegate, int32, ButtonIndex);

/**
 * UPGPopup - 모달/모달리스 팝업 위젯 클래스
 */
UCLASS(Blueprintable)
class UPLAYGROUND_API UPGPopup : public UPGWidgetBase
{
    GENERATED_BODY()

public:
    UPGPopup(const FObjectInitializer& ObjectInitializer);

    // 팝업 설정 함수
    UFUNCTION(BlueprintCallable, Category = "Popup")
    void SetupPopup(const FText& InTitle, const FText& InMessage, EPGPopupType InType = EPGPopupType::Default);
    
    // 팝업 모드 설정 (모달/모달리스)
    UFUNCTION(BlueprintCallable, Category = "Popup")
    void SetPopupMode(EPGPopupMode InMode);
    
    // 버튼 추가
    UFUNCTION(BlueprintCallable, Category = "Popup")
    void AddButton(const FText& ButtonText, int32 ButtonIndex);
    
    // 팝업 표시 및 숨김 오버라이드
    virtual void ShowWidget() override;
    virtual void HideWidget() override;

    // 버튼 클릭 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Popup")
    FOnPopupButtonClickedDelegate OnPopupButtonClicked;

protected:
    // UUserWidget 오버라이드
    virtual void NativeConstruct() override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
    
    // 타이틀 및 메시지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup")
    FText PopupTitle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup")
    FText PopupMessage;
    
    // 팝업 타입 및 모드
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup")
    EPGPopupType PopupType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup")
    EPGPopupMode PopupMode;
    
    // 배경 딤(dim) 효과 강도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Popup", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float BackgroundDimOpacity;
    
    // UI 엘리먼트
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Popup")
    class UTextBlock* TitleTextBlock;
    
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Popup")
    class UTextBlock* MessageTextBlock;
    
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Popup")
    class UPanelWidget* ButtonContainer;
    
    UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "Popup")
    class UImage* BackgroundDim;
    
    // 버튼 클릭 핸들러
    UFUNCTION()
    void OnButtonClicked(int32 ButtonIndex);
    
    // 버튼 위젯 생성
    UFUNCTION(BlueprintImplementableEvent, Category = "Popup")
    class UButton* CreateButtonWidget(const FText& ButtonText, int32 ButtonIndex);
    
    // 팝업 타입에 따른 스타일 적용
    UFUNCTION(BlueprintImplementableEvent, Category = "Popup")
    void ApplyPopupStyle(EPGPopupType InType);
};