#include "UI/Widgets/Elements/PGMenuWidget.h"
#include "UI/Widgets/Elements/PGMenuButtonWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Animation/WidgetAnimation.h"

UPGMenuWidget::UPGMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    // 생성자 기본 구현 - FObjectInitializer 사용
}

void UPGMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // 토글 버튼 이벤트 바인딩
    if (MenuToggleButton)
    {
        MenuToggleButton->OnClicked.AddDynamic(this, &UPGMenuWidget::OnToggleButtonClicked);
    }
    
    // 초기 상태 설정
    if (MenuPanel)
    {
        MenuPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
    
    bIsMenuOpen = false;
}

void UPGMenuWidget::UpdateWidget()
{
    Super::UpdateWidget();
    
    // 기본 메뉴 항목 설정
    TArray<FPGMenuItemData> DefaultMenuItems;
    
    // 기본 메뉴 항목 (예시)
    FPGMenuItemData CharacterItem;
    CharacterItem.MenuID = TEXT("Character");
    CharacterItem.MenuName = FText::FromString(TEXT("캐릭터"));
    CharacterItem.ActionName = TEXT("OpenCharacterWindow");
    
    FPGMenuItemData InventoryItem;
    InventoryItem.MenuID = TEXT("Inventory");
    InventoryItem.MenuName = FText::FromString(TEXT("인벤토리"));
    InventoryItem.ActionName = TEXT("OpenInventoryWindow");
    
    FPGMenuItemData QuestItem;
    QuestItem.MenuID = TEXT("Quest");
    QuestItem.MenuName = FText::FromString(TEXT("퀘스트"));
    QuestItem.ActionName = TEXT("OpenQuestWindow");
    
    FPGMenuItemData SettingsItem;
    SettingsItem.MenuID = TEXT("Settings");
    SettingsItem.MenuName = FText::FromString(TEXT("설정"));
    SettingsItem.ActionName = TEXT("OpenSettingsWindow");
    
    FPGMenuItemData ExitItem;
    ExitItem.MenuID = TEXT("Exit");
    ExitItem.MenuName = FText::FromString(TEXT("게임종료"));
    ExitItem.ActionName = TEXT("ExitGame");
    
    DefaultMenuItems.Add(CharacterItem);
    DefaultMenuItems.Add(InventoryItem);
    DefaultMenuItems.Add(QuestItem);
    DefaultMenuItems.Add(SettingsItem);
    DefaultMenuItems.Add(ExitItem);
    
    SetMenuItems(DefaultMenuItems);
}

void UPGMenuWidget::SetMenuItems(const TArray<FPGMenuItemData>& MenuItems)
{
    CurrentMenuItems = MenuItems;
    CreateMenuButtons();
}

void UPGMenuWidget::CreateMenuButtons()
{
    // 컨테이너 확인
    if (!MenuButtonsContainer || !MenuButtonWidgetClass)
    {
        return;
    }
    
    // 기존 버튼 제거
    MenuButtonsContainer->ClearChildren();
    
    // 새 버튼 생성
    for (const FPGMenuItemData& MenuItem : CurrentMenuItems)
    {
        UPGMenuButtonWidget* ButtonWidget = CreateWidget<UPGMenuButtonWidget>(this, MenuButtonWidgetClass);
        if (ButtonWidget)
        {
            // 버튼 설정
            ButtonWidget->SetButtonData(MenuItem);
            
            // 클릭 이벤트 바인딩
            ButtonWidget->OnMenuButtonClicked.AddDynamic(this, &UPGMenuWidget::OnMenuButtonClicked);
            
            // 컨테이너에 추가
            MenuButtonsContainer->AddChild(ButtonWidget);
        }
    }
}

void UPGMenuWidget::ToggleMenu()
{
    if (bIsMenuOpen)
    {
        CloseMenu();
    }
    else
    {
        OpenMenu();
    }
}

void UPGMenuWidget::OpenMenu()
{
    if (bIsMenuOpen)
    {
        return;
    }
    
    bIsMenuOpen = true;
    
    if (MenuPanel)
    {
        MenuPanel->SetVisibility(ESlateVisibility::Visible);
    }
    
    // 애니메이션 재생
    if (MenuOpenAnimation)
    {
        PlayAnimation(MenuOpenAnimation);
    }
}

void UPGMenuWidget::CloseMenu()
{
    if (!bIsMenuOpen)
    {
        return;
    }
    
    bIsMenuOpen = false;
    
    // 애니메이션 재생
    if (MenuCloseAnimation)
    {
        PlayAnimation(MenuCloseAnimation, 0.0f, 1, EUMGSequencePlayMode::Forward, 1.0f, false);
        
        // 애니메이션 종료 후 호출될 함수 설정
        float AnimDuration = MenuCloseAnimation->GetEndTime();
        
        // 타이머 설정 - FTimerManager의 적절한 오버로드 사용
        if (GetWorld())
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                FTimerDelegate::CreateUObject(this, &UPGMenuWidget::OnCloseAnimationFinished),
                AnimDuration,
                false
            );
        }
        else
        {
            // World가 없는 경우 바로 숨기기
            OnCloseAnimationFinished();
        }
    }
    else
    {
        // 애니메이션 없으면 바로 숨기기
        OnCloseAnimationFinished();
    }
}

void UPGMenuWidget::OnCloseAnimationFinished()
{
    if (MenuPanel)
    {
        MenuPanel->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UPGMenuWidget::OnMenuButtonClicked(FName MenuID)
{
    // 메뉴 항목 선택 이벤트 발송
    OnMenuItemSelected.Broadcast(MenuID);
    
    // 메뉴 닫기 (선택적)
    // CloseMenu();
}

void UPGMenuWidget::OnToggleButtonClicked()
{
    ToggleMenu();
}