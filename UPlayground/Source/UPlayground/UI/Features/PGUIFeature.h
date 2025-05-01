#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "PGUIFeature.generated.h"

UCLASS()
class UPLAYGROUND_API UPGUIFeature : public UGameFeatureAction
{
    GENERATED_BODY()
    
public:
    UPGUIFeature();
    
    // GameFeatureAction 인터페이스 구현 - 수정된 시그니처
    virtual void OnGameFeatureRegistering() override;
    virtual void OnGameFeatureUnregistering() override;
    virtual void OnGameFeatureActivating() override;
    virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
    
    // HUD 요소 구성 설정
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TArray<struct FPGHUDElementConfig> HUDElementConfigs;
    
    // UI 매니저 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UPGUIManager> UIManagerClass;
    
protected:
    // 매니저 인스턴스 추적
    UPROPERTY(Transient)
    TObjectPtr<class UPGUIManager> UIManagerInstance;
};