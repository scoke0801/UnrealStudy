#include "UI/Widgets/Elements/PGMenuButtonWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UPGMenuButtonWidget::UPGMenuButtonWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGMenuButtonWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 버튼 클릭 이벤트 바인딩
    if (MenuButton)
    {
        MenuButton->OnClicked.AddDynamic(this, &UPGMenuButtonWidget::OnButtonClicked);
    }
}

void UPGMenuButtonWidget::SetButtonData(const FPGMenuItemData& MenuItemData)
{
    MenuID = MenuItemData.MenuID;
    ActionName = MenuItemData.ActionName;
    
    // 아이콘 설정
    if (MenuIcon && MenuItemData.MenuIcon)
    {
        MenuIcon->SetBrushFromTexture(MenuItemData.MenuIcon);
        MenuIcon->SetVisibility(ESlateVisibility::Visible);
    }
    else if (MenuIcon)
    {
        MenuIcon->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    // 텍스트 설정
    if (MenuText)
    {
        MenuText->SetText(MenuItemData.MenuName);
    }
}

void UPGMenuButtonWidget::OnButtonClicked()
{
    OnMenuButtonClicked.Broadcast(MenuID);
}