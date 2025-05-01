#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Base/PGHUDBaseWidget.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGCharacterInfoWidget.generated.h"

UCLASS()
class UPLAYGROUND_API UPGCharacterInfoWidget : public UPGHUDBaseWidget
{
    GENERATED_BODY()
    
public:
    UPGCharacterInfoWidget(const FObjectInitializer& ObjectInitializer);
    
    // 위젯 업데이트 오버라이드
    virtual void UpdateWidget() override;
    
    // 캐릭터 정보 업데이트 함수
    UFUNCTION(BlueprintCallable, Category = "Character Info")
    void UpdateCharacterImage(UTexture2D* CharacterImage);
    
    UFUNCTION(BlueprintCallable, Category = "Character Info")
    void UpdateHealthBar(float CurrentHealth, float MaxHealth);
    
    UFUNCTION(BlueprintCallable, Category = "Character Info")
    void UpdateExperienceBar(float CurrentExp, float MaxExp, int32 Level);
    
    // 버프/디버프 관리 함수
    UFUNCTION(BlueprintCallable, Category = "Character Info")
    void AddBuff(const FPGBuffData& BuffData);
    
    UFUNCTION(BlueprintCallable, Category = "Character Info")
    void RemoveBuff(FName BuffID);
    
    UFUNCTION(BlueprintCallable, Category = "Character Info")
    void UpdateBuffDuration(FName BuffID, float RemainingTime);
    
    UFUNCTION(BlueprintCallable, Category = "Character Info")
    void UpdateBuffStack(FName BuffID, int32 StackCount);
    
    UFUNCTION(BlueprintCallable, Category = "Character Info")
    void ClearAllBuffs();
    
protected:
    // 틱 업데이트
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
    // UI 요소 참조
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UImage> CharacterImage;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UProgressBar> HealthBar;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> HealthText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UProgressBar> ExperienceBar;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> LevelText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UTextBlock> CharacterNameText;
    
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<class UWrapBox> BuffContainer;
    
    // 버프 위젯 클래스
    UPROPERTY(EditDefaultsOnly, Category = "Character Info")
    TSubclassOf<class UPGBuffWidget> BuffWidgetClass;
    
    // 현재 적용된 버프/디버프 트래킹
    UPROPERTY()
    TMap<FName, FPGBuffData> ActiveBuffs;
    
    UPROPERTY()
    TMap<FName, TObjectPtr<class UPGBuffWidget>> BuffWidgets;
    
private:
    // 내부 함수
    void UpdateBuffsDisplay();
};