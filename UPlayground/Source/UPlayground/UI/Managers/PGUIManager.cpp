#include "UI/Managers/PGUIManager.h"
#include "UI/Managers/PGHUDManager.h"
#include "GameFramework/Actor.h"

UPGUIManager::UPGUIManager()
{
    ManagerName = TEXT("UIManager");
}

bool UPGUIManager::Initialize()
{
    bool bResult = Super::Initialize();
    
    // HUD 매니저 생성
    HUDManager = NewObject<UPGHUDManager>(this);
    if (HUDManager)
    {
        HUDManager->Initialize();
    }
    
    InitializeUISystem();
    
    return bResult;
}

void UPGUIManager::Shutdown()
{
    if (HUDManager)
    {
        HUDManager->Deinitialize();
        HUDManager = nullptr;
    }
    
    Super::Shutdown();
}

void UPGUIManager::InitializeUISystem_Implementation()
{
    // UI 시스템 초기화 로직
    UE_LOG(LogTemp, Log, TEXT("UI System Initialized"));
}

UPGHUDManager* UPGUIManager::GetHUDManager_Implementation()
{
    return HUDManager;
}

bool UPGUIManager::IsActorBeingDestroyed(AActor* Actor) const
{
    if (!Actor)
    {
        return true;
    }
    
    return Actor->IsActorBeingDestroyed();
}

bool UPGUIManager::IsActorBeingDestroyed() const
{
    // 매니저 자체는 UObject이므로 IsPendingKill() 또는 HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed)로 확인
    return HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed);
}