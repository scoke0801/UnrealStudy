#pragma once

#include "CoreMinimal.h"
#include "Managers/PGBaseManager.h"
#include "UI/Managers/IPGIUIManager.h"
#include "PGUIManager.generated.h"

UCLASS(Blueprintable)
class UPLAYGROUND_API UPGUIManager : public UPGBaseManager, public IPGIUIManager
{
    GENERATED_BODY()
    
public:
    UPGUIManager();
    
    // PGBaseManager 인터페이스 구현
    virtual bool Initialize() override;
    virtual void Shutdown() override;
    
    // IPGIUIManager 인터페이스 구현
    virtual void InitializeUISystem_Implementation() override;
    virtual class UPGHUDManager* GetHUDManager_Implementation() override;
    
    // 액터 소멸 여부 확인 유틸리티 함수
    bool IsActorBeingDestroyed(AActor* Actor) const;
    
    // 매니저 자체의 소멸 여부 확인 (인수 없는 버전)
    bool IsActorBeingDestroyed() const;
    
protected:
    // HUD 매니저 인스턴스
    UPROPERTY()
    TObjectPtr<class UPGHUDManager> HUDManager;
};