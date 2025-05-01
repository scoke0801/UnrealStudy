#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPGIUIManager.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UPGIUIManager : public UInterface
{
    GENERATED_BODY()
};

class UPLAYGROUND_API IPGIUIManager
{
    GENERATED_BODY()

public:
    // UI 시스템 초기화
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
    void InitializeUISystem();
    
    // HUD 매니저 가져오기
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "UI")
    class UPGHUDManager* GetHUDManager();
};