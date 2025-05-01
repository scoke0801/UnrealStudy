#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PGPlayerControllerHUD.generated.h"

/**
 * 퀵슬롯 및 HUD 지원을 포함한 확장 플레이어 컨트롤러
 * 기존 플레이어 컨트롤러에서 상속해서 사용하거나, 필요한 코드를 기존 컨트롤러에 추가하세요.
 */
UCLASS()
class UPLAYGROUND_API APGPlayerControllerHUD : public APlayerController
{
    GENERATED_BODY()
    
public:
    APGPlayerControllerHUD();
    
    // HUD 컴포넌트 가져오기
    UFUNCTION(BlueprintPure, Category = "UI")
    class UPGHUDComponent* GetHUDComponent() const;
    
protected:
    virtual void BeginPlay() override;
    
    // HUD 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class UPGHUDComponent> HUDComponent;
};