#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGBuffWidget.generated.h"

UCLASS()
class UPLAYGROUND_API UPGBuffWidget : public UUserWidget
{
    GENERATED_BODY()
    
public:
    UPGBuffWidget(const FObjectInitializer& ObjectInitializer);
    
    // 버프 데이터 업데이트
    UFUNCTION(BlueprintCallable, Category = "Buff")
    void UpdateBuffData(const FPGBuffData& BuffData);
    
    // 특정 필드 업데이트
    UFUNCTION(BlueprintCallable, Category = "Buff")
    void UpdateBuffDuration(float RemainingTime);
    
    UFUNCTION(BlueprintCallable, Category = "Buff")
    void UpdateBuffStack(int32 StackCount);
    
protected:
    // 틱 업데이트
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
    // UI 요소
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UImage> BuffIcon;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> StackText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> TimeRemainingText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UProgressBar> DurationBar;
    
    // 현재 버프 데이터
    UPROPERTY(BlueprintReadOnly, Category = "Buff")
    FPGBuffData CurrentBuffData;
    
    // 툴팁 설정
    UFUNCTION(BlueprintCallable, Category = "Buff")
    void SetBuffTooltip();
};