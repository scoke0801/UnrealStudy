#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGHUDComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UPLAYGROUND_API UPGHUDComponent : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UPGHUDComponent();
    
    // 컴포넌트 초기화/해제 기능
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    // HUD 설정
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetupHUD();
    
    // HUD 컨테이너 위젯 가져오기
    UFUNCTION(BlueprintPure, Category = "HUD")
    class UPGHUDContainerWidget* GetHUDContainer() const;
    
    // HUD 매니저 가져오기
    UFUNCTION(BlueprintPure, Category = "HUD")
    class UPGHUDManager* GetHUDManager() const;
    
    // HUD 요소 표시/숨김
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetHUDElementVisibility(FName ElementID, bool bVisible);
    
    // HUD 요소 가져오기
    UFUNCTION(BlueprintCallable, Category = "HUD")
    class UPGHUDBaseWidget* GetHUDElement(FName ElementID);
    
protected:
    // HUD 컨테이너 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TSubclassOf<class UPGHUDContainerWidget> HUDContainerClass;
    
    // 기본 HUD 요소 설정
    UPROPERTY(EditDefaultsOnly, Category = "HUD")
    TArray<FPGHUDElementConfig> DefaultHUDElements;
    
    // HUD 컨테이너 위젯 인스턴스
    UPROPERTY()
    TObjectPtr<class UPGHUDContainerWidget> HUDContainerInstance;
    
    // HUD 매니저 캐싱
    UPROPERTY()
    TObjectPtr<class UPGHUDManager> CachedHUDManager;
    
    // 게임 모드 변경 처리
    UFUNCTION()
    void HandleGameModeChanged(bool bIsInGameMode);
};