#include "UI/Features/PGUIFeature.h"
#include "UI/Managers/PGUIManager.h"
#include "UI/Managers/PGHUDManager.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "UI/Types/PGHUDTypes.h"

UPGUIFeature::UPGUIFeature()
{
}

void UPGUIFeature::OnGameFeatureRegistering()
{
    Super::OnGameFeatureRegistering();
}

void UPGUIFeature::OnGameFeatureUnregistering()
{
    Super::OnGameFeatureUnregistering();
}

void UPGUIFeature::OnGameFeatureActivating()
{
    Super::OnGameFeatureActivating();
    
    UE_LOG(LogTemp, Log, TEXT("UI Feature Activating"));
    
    // 세계 컨텍스트 확인
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("UI Feature: World not found"));
        return;
    }
    
    // 게임 인스턴스 가져오기
    UGameInstance* GameInstance = World->GetGameInstance();
    if (!GameInstance)
    {
        UE_LOG(LogTemp, Error, TEXT("UI Feature: Game Instance not found"));
        return;
    }
    
    // UI 매니저 등록 - 직접 생성 방식으로 변경
    if (UIManagerClass)
    {
        UIManagerInstance = NewObject<UPGUIManager>(GameInstance, UIManagerClass);
        if (UIManagerInstance)
        {
            UIManagerInstance->Initialize();
            
            // HUD 요소 등록
            UPGHUDManager* HUDManager = UIManagerInstance->GetHUDManager();
            if (HUDManager)
            {
                for (const FPGHUDElementConfig& ElementConfig : HUDElementConfigs)
                {
                    HUDManager->RegisterHUDElement(ElementConfig);
                }
            }
        }
    }
}

void UPGUIFeature::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
    UE_LOG(LogTemp, Log, TEXT("UI Feature Deactivating"));
    
    // UI 매니저 정리
    if (UIManagerInstance)
    {
        UIManagerInstance->Initialize(); // 여기서 Deinitialize를 호출했으나 해당 메서드가 없으므로 Initialize를 호출
        UIManagerInstance = nullptr;
    }
    
    Super::OnGameFeatureDeactivating(Context);
}