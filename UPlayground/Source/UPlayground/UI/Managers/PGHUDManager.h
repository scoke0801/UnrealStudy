#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGHUDManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHUDElementVisibilityChanged, FName, ElementID, bool, bIsVisible);

UCLASS(Blueprintable)
class UPLAYGROUND_API UPGHUDManager : public UObject
{
    GENERATED_BODY()
    
public:
    UPGHUDManager();
    
    // 초기화/해제
    void Initialize();
    void Deinitialize();
    
    // HUD 컨테이너 설정
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetHUDContainer(class UPGHUDContainerWidget* InHUDContainer);
    
    // HUD 요소 등록
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void RegisterHUDElement(const FPGHUDElementConfig& ElementConfig);
    
    // HUD 요소 표시/숨김
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetHUDElementVisibility(FName ElementID, bool bVisible);
    
    // HUD 요소 가져오기
    UFUNCTION(BlueprintCallable, Category = "HUD")
    class UPGHUDBaseWidget* GetHUDElement(FName ElementID);
    
    // 모든 HUD 요소 표시/숨김
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetAllHUDElementsVisibility(bool bVisible);
    
    // 게임 모드 변경에 따른 HUD 요소 표시/숨김 업데이트
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateHUDVisibilityForGameMode(bool bIsInGameMode);
    
public:
    // 이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "HUD|Events")
    FOnHUDElementVisibilityChanged OnHUDElementVisibilityChanged;
    
protected:
    // HUD 컨테이너 위젯
    UPROPERTY()
    TObjectPtr<class UPGHUDContainerWidget> HUDContainer;
    
    // 등록된 HUD 요소 설정
    UPROPERTY()
    TMap<FName, FPGHUDElementConfig> RegisteredHUDElements;
    
    // 생성된 HUD 위젯 인스턴스
    UPROPERTY()
    TMap<FName, TObjectPtr<class UPGHUDBaseWidget>> HUDWidgetInstances;
};