#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PGHUDBaseWidget.generated.h"

UCLASS()
class UPLAYGROUND_API UPGHUDBaseWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPGHUDBaseWidget(const FObjectInitializer& ObjectInitializer);
    
    // 요소 ID 설정/조회
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void SetElementID(const FName& InElementID);
    
    UFUNCTION(BlueprintPure, Category = "HUD")
    FName GetElementID() const;
    
    // NativeConstruct 오버라이드
    virtual void NativeConstruct() override;
    
    // 위젯 업데이트
    UFUNCTION(BlueprintCallable, Category = "HUD")
    virtual void UpdateWidget();
    
protected:
    // 요소 ID
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
    FName ElementID;
};