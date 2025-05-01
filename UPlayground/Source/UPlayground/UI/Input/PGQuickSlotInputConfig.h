#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputAction.h"
#include "PGQuickSlotInputConfig.generated.h"

UCLASS()
class UPLAYGROUND_API UPGQuickSlotInputConfig : public UDataAsset
{
    GENERATED_BODY()
    
public:
    // 퀵슬롯 숫자 키 입력 (1-9, 0)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TArray<TObjectPtr<UInputAction>> QuickSlotNumberActions;
    
    // 토글 입력 액션
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> ToggleInventoryAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> ToggleQuestAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> ToggleMenuAction;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    TObjectPtr<UInputAction> ToggleMinimapAction;
};