#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/Types/PGHUDTypes.h"
#include "PGHUDConfigDataAsset.generated.h"

UCLASS()
class UPLAYGROUND_API UPGHUDConfigDataAsset : public UDataAsset
{
    GENERATED_BODY()
    
public:
    // HUD 요소 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Configuration")
    TArray<FPGHUDPositionConfig> HUDElementConfigs;
    
    // 퀵슬롯 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Configuration")
    int32 DefaultQuickSlotCount = 10;
    
    // 메뉴 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Configuration")
    TArray<FPGMenuItemData> DefaultMenuItems;
    
    // 미니맵 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Configuration")
    float DefaultMinimapZoom = 1.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD Configuration")
    bool bShowMinimapByDefault = true;
};