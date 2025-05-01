#include "UI/Managers/PGHUDManager.h"
#include "UI/Widgets/Base/PGHUDContainerWidget.h"
#include "UI/Widgets/Base/PGHUDBaseWidget.h"

UPGHUDManager::UPGHUDManager()
{
}

void UPGHUDManager::Initialize()
{
    UE_LOG(LogTemp, Log, TEXT("HUD Manager Initialized"));
}

void UPGHUDManager::Deinitialize()
{
    // 위젯 인스턴스 정리
    HUDWidgetInstances.Empty();
    RegisteredHUDElements.Empty();
    HUDContainer = nullptr;
}

void UPGHUDManager::SetHUDContainer(UPGHUDContainerWidget* InHUDContainer)
{
    HUDContainer = InHUDContainer;
    
    // 이미 등록된 HUD 요소들을 컨테이너에 추가
    for (const auto& ElementPair : RegisteredHUDElements)
    {
        const FName& ElementID = ElementPair.Key;
        const FPGHUDElementConfig& Config = ElementPair.Value;
        
        // 아직 생성되지 않은 위젯만 생성
        if (!HUDWidgetInstances.Contains(ElementID) && Config.WidgetClass)
        {
            UPGHUDBaseWidget* NewWidget = CreateWidget<UPGHUDBaseWidget>(HUDContainer->GetOwningPlayer(), Config.WidgetClass);
            if (NewWidget)
            {
                NewWidget->SetElementID(ElementID);
                HUDWidgetInstances.Add(ElementID, NewWidget);
                
                // 컨테이너에 위젯 추가
                HUDContainer->AddHUDElement(NewWidget, Config.Layer);
                
                // 초기 가시성 설정
                const bool bInitialVisibility = (Config.DefaultVisibility == EPGHUDVisibility::Visible || 
                                              Config.DefaultVisibility == EPGHUDVisibility::VisibleOnlyInGame);
                NewWidget->SetVisibility(bInitialVisibility ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
            }
        }
    }
}

void UPGHUDManager::RegisterHUDElement(const FPGHUDElementConfig& ElementConfig)
{
    // ID가 유효한지 확인
    if (ElementConfig.ElementID.IsNone() || !ElementConfig.WidgetClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid HUD Element Configuration"));
        return;
    }
    
    // 설정 등록
    RegisteredHUDElements.Add(ElementConfig.ElementID, ElementConfig);
    
    // 컨테이너가 이미 있으면 위젯 즉시 생성
    if (HUDContainer)
    {
        UPGHUDBaseWidget* NewWidget = CreateWidget<UPGHUDBaseWidget>(HUDContainer->GetOwningPlayer(), ElementConfig.WidgetClass);
        if (NewWidget)
        {
            NewWidget->SetElementID(ElementConfig.ElementID);
            HUDWidgetInstances.Add(ElementConfig.ElementID, NewWidget);
            
            // 컨테이너에 위젯 추가
            HUDContainer->AddHUDElement(NewWidget, ElementConfig.Layer);
            
            // 초기 가시성 설정
            const bool bInitialVisibility = (ElementConfig.DefaultVisibility == EPGHUDVisibility::Visible || 
                                          ElementConfig.DefaultVisibility == EPGHUDVisibility::VisibleOnlyInGame);
            NewWidget->SetVisibility(bInitialVisibility ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
        }
    }
}

void UPGHUDManager::SetHUDElementVisibility(FName ElementID, bool bVisible)
{
    UPGHUDBaseWidget* Widget = GetHUDElement(ElementID);
    if (Widget)
    {
        Widget->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
        OnHUDElementVisibilityChanged.Broadcast(ElementID, bVisible);
    }
}

UPGHUDBaseWidget* UPGHUDManager::GetHUDElement(FName ElementID)
{
    return HUDWidgetInstances.Contains(ElementID) ? HUDWidgetInstances[ElementID] : nullptr;
}

void UPGHUDManager::SetAllHUDElementsVisibility(bool bVisible)
{
    for (const auto& WidgetPair : HUDWidgetInstances)
    {
        const FName& ElementID = WidgetPair.Key;
        UPGHUDBaseWidget* Widget = WidgetPair.Value;
        
        if (Widget)
        {
            Widget->SetVisibility(bVisible ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
            OnHUDElementVisibilityChanged.Broadcast(ElementID, bVisible);
        }
    }
}

void UPGHUDManager::UpdateHUDVisibilityForGameMode(bool bIsInGameMode)
{
    for (const auto& ElementPair : RegisteredHUDElements)
    {
        const FName& ElementID = ElementPair.Key;
        const FPGHUDElementConfig& Config = ElementPair.Value;
        
        bool bShouldBeVisible = false;
        
        switch (Config.DefaultVisibility)
        {
            case EPGHUDVisibility::Visible:
                bShouldBeVisible = true;
                break;
            case EPGHUDVisibility::Hidden:
                bShouldBeVisible = false;
                break;
            case EPGHUDVisibility::VisibleOnlyInGame:
                bShouldBeVisible = bIsInGameMode;
                break;
            case EPGHUDVisibility::VisibleOnlyInMenu:
                bShouldBeVisible = !bIsInGameMode;
                break;
        }
        
        SetHUDElementVisibility(ElementID, bShouldBeVisible);
    }
}