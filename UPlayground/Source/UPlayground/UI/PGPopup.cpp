// PGPopup.cpp
#include "PGPopup.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UPGPopup::UPGPopup(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , PopupType(EPGPopupType::Default)
    , PopupMode(EPGPopupMode::Modal)
    , BackgroundDimOpacity(0.5f)
{
    // 팝업은 높은 Z-오더를 가짐
    ZOrder = 100;
}

void UPGPopup::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 바인딩된 위젯 초기화
    if (TitleTextBlock)
    {
        TitleTextBlock->SetText(PopupTitle);
    }
    
    if (MessageTextBlock)
    {
        MessageTextBlock->SetText(PopupMessage);
    }
    
    if (BackgroundDim)
    {
        FLinearColor DimColor = FLinearColor(0.0f, 0.0f, 0.0f, BackgroundDimOpacity);
        BackgroundDim->SetColorAndOpacity(DimColor);
        BackgroundDim->SetVisibility(PopupMode == EPGPopupMode::Modal ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void UPGPopup::SetupPopup(const FText& InTitle, const FText& InMessage, EPGPopupType InType)
{
    PopupTitle = InTitle;
    PopupMessage = InMessage;
    PopupType = InType;
    
    if (TitleTextBlock)
    {
        TitleTextBlock->SetText(PopupTitle);
    }
    
    if (MessageTextBlock)
    {
        MessageTextBlock->SetText(PopupMessage);
    }
    
    // 스타일 적용
    ApplyPopupStyle(PopupType);
}

void UPGPopup::SetPopupMode(EPGPopupMode InMode)
{
    PopupMode = InMode;
    
    if (BackgroundDim)
    {
        BackgroundDim->SetVisibility(PopupMode == EPGPopupMode::Modal ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

void UPGPopup::AddButton(const FText& ButtonText, int32 ButtonIndex)
{
    if (ButtonContainer)
    {
        // 블루프린트 구현 이벤트를 호출하여 버튼 위젯 생성
        UButton* ButtonWidget = CreateButtonWidget(ButtonText, ButtonIndex);
        
        if (ButtonWidget)
        {
            // 버튼 클릭 이벤트 바인딩
            FScriptDelegate ButtonDelegate;
            ButtonDelegate.BindUFunction(this, "OnButtonClicked");

            ButtonWidget->OnClicked.Add(ButtonDelegate);
            
            // 버튼 컨테이너에 추가
            ButtonContainer->AddChild(ButtonWidget);
        }
    }
}

void UPGPopup::ShowWidget()
{
    Super::ShowWidget();
    
    // 모달 팝업의 경우 입력 포커스 가져오기
    if (PopupMode == EPGPopupMode::Modal)
    {
        UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this);
    }
}

void UPGPopup::HideWidget()
{
    Super::HideWidget();
    
    // 모달 팝업의 경우 입력 모드 복원
    if (PopupMode == EPGPopupMode::Modal)
    {
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(GetOwningPlayer());
    }
}

void UPGPopup::OnButtonClicked(int32 ButtonIndex)
{
    // 버튼 클릭 델리게이트 호출
    OnPopupButtonClicked.Broadcast(ButtonIndex);
    
    // 팝업 닫기 (필요에 따라 옵션화 가능)
    HideWidget();
}

FReply UPGPopup::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
    // 모달 팝업의 경우 모든 입력 처리
    if (PopupMode == EPGPopupMode::Modal)
    {
        return FReply::Handled();
    }
    
    return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}