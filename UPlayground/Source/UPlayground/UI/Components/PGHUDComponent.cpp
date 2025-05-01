#include "UI/Components/PGHUDComponent.h"
#include "UI/Widgets/Base/PGHUDContainerWidget.h"
#include "UI/Managers/PGHUDManager.h"
#include "UI/Managers/PGUIManager.h"
#include "GameFramework/PlayerController.h"

UPGHUDComponent::UPGHUDComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPGHUDComponent::BeginPlay()
{
    Super::BeginPlay();
    
    SetupHUD();
}

void UPGHUDComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // HUD 정리
    if (HUDContainerInstance)
    {
        HUDContainerInstance->RemoveFromParent();
        HUDContainerInstance = nullptr;
    }
    
    CachedHUDManager = nullptr;
    
    Super::EndPlay(EndPlayReason);
}

void UPGHUDComponent::SetupHUD()
{
    // 플레이어 컨트롤러 가져오기
    APlayerController* PC = Cast<APlayerController>(GetOwner());
    if (!PC)
    {
        UE_LOG(LogTemp, Error, TEXT("HUD Component can only be added to a PlayerController"));
        return;
    }
    
    // 게임 인스턴스 가져오기
    UGameInstance* GameInstance = PC->GetGameInstance();
    if (!GameInstance)
    {
        return;
    }
    
    // UI 매니저 찾기 - 직접 찾기 방식으로 변경
    UPGUIManager* UIManager = nullptr;
    for (TObjectIterator<UPGUIManager> It; It; ++It)
    {
        UIManager = *It;
        if (UIManager && !UIManager->IsActorBeingDestroyed())
        {
            break;
        }
    }
    
    if (!UIManager)
    {
        UE_LOG(LogTemp, Error, TEXT("UI Manager not found"));
        // 필요한 경우 여기서 직접 생성할 수 있습니다.
        return;
    }
    
    // HUD 매니저 가져오기
    CachedHUDManager = UIManager->GetHUDManager();
    if (!CachedHUDManager)
    {
        UE_LOG(LogTemp, Error, TEXT("HUD Manager not found"));
        return;
    }
    
    // HUD 컨테이너 위젯 생성
    if (HUDContainerClass)
    {
        HUDContainerInstance = CreateWidget<UPGHUDContainerWidget>(PC, HUDContainerClass);
        if (HUDContainerInstance)
        {
            // 위젯 추가
            HUDContainerInstance->AddToViewport();
            
            // HUD 매니저에 컨테이너 설정
            CachedHUDManager->SetHUDContainer(HUDContainerInstance);
            
            // 기본 HUD 요소 등록
            for (const FPGHUDElementConfig& ElementConfig : DefaultHUDElements)
            {
                CachedHUDManager->RegisterHUDElement(ElementConfig);
            }
        }
    }
}

UPGHUDContainerWidget* UPGHUDComponent::GetHUDContainer() const
{
    return HUDContainerInstance;
}

UPGHUDManager* UPGHUDComponent::GetHUDManager() const
{
    return CachedHUDManager;
}

void UPGHUDComponent::SetHUDElementVisibility(FName ElementID, bool bVisible)
{
    if (CachedHUDManager)
    {
        CachedHUDManager->SetHUDElementVisibility(ElementID, bVisible);
    }
}

UPGHUDBaseWidget* UPGHUDComponent::GetHUDElement(FName ElementID)
{
    if (CachedHUDManager)
    {
        return CachedHUDManager->GetHUDElement(ElementID);
    }
    
    return nullptr;
}

void UPGHUDComponent::HandleGameModeChanged(bool bIsInGameMode)
{
    if (CachedHUDManager)
    {
        CachedHUDManager->UpdateHUDVisibilityForGameMode(bIsInGameMode);
    }
}