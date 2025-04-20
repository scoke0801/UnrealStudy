#pragma once

#include "CoreMinimal.h"
#include "../Managers/PGGameFeatureManagerAction.h"
#include "PGPlayerSystemFeature.generated.h"

/**
 * 플레이어 시스템 기능을 위한 게임 피쳐 매니저
 */
UCLASS()
class UPLAYGROUND_API UPGPlayerSystemFeature : public UPGGameFeatureManagerAction
{
    GENERATED_BODY()
    
public:
    UPGPlayerSystemFeature();
    
    // 게임 피쳐 라이프사이클
    virtual void OnGameFeatureActivating() override;
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
    
protected:
    // 등록된 매니저 인스턴스
    UPROPERTY()
    TArray<class UPGBaseManager*> RegisteredManagers;
    
    // 매니저 서브시스템 참조
    UPROPERTY()
    class UPGManagerSubsystem* ManagerSubsystem;
};
