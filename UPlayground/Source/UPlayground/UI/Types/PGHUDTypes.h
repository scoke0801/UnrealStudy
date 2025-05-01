#pragma once

#include "CoreMinimal.h"
#include "PGHUDTypes.generated.h"

UENUM(BlueprintType)
enum class EPGHUDVisibility : uint8
{
    Visible,
    Hidden,
    VisibleOnlyInGame,
    VisibleOnlyInMenu
};

UENUM(BlueprintType)
enum class EPGHUDLayer : uint8
{
    Background,
    GameLayer,
    Foreground,
    Modal,
    COUNT
};

UENUM(BlueprintType)
enum class EPGHUDCornerPosition : uint8
{
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight,
    Center
};

UENUM(BlueprintType)
enum class EPGHUDElementType : uint8
{
    CharacterInfo,
    Currency,
    Inventory,
    Quest,
    QuickSlot,
    Minimap,
    Menu,
    Other
};

USTRUCT(BlueprintType)
struct FPGHUDElementConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UPGHUDBaseWidget> WidgetClass;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGHUDLayer Layer = EPGHUDLayer::GameLayer;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGHUDVisibility DefaultVisibility = EPGHUDVisibility::Visible;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ElementID;
};

// 버프/디버프 타입
UENUM(BlueprintType)
enum class EPGBuffType : uint8
{
    Positive,
    Negative,
    Neutral
};

// 버프/디버프 데이터 구조체
USTRUCT(BlueprintType)
struct FPGBuffData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName BuffID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText BuffName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText BuffDescription;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* BuffIcon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGBuffType BuffType = EPGBuffType::Neutral;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RemainingTime = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StackCount = 1;
};

// 퀵슬롯 항목 타입
UENUM(BlueprintType)
enum class EPGQuickSlotItemType : uint8
{
    None,
    Skill,
    Consumable,
    Equipment,
    Other
};

// 퀵슬롯 항목 데이터 구조체
USTRUCT(BlueprintType)
struct FPGQuickSlotItemData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText ItemName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemIcon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGQuickSlotItemType ItemType = EPGQuickSlotItemType::None;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cooldown = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RemainingCooldown = 0.0f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Quantity = 0;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsAvailable = true;
};

// 메뉴 항목 데이터 구조체
USTRUCT(BlueprintType)
struct FPGMenuItemData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName MenuID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText MenuName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* MenuIcon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ActionName;
};

USTRUCT(BlueprintType)
struct FPGHUDPositionConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGHUDElementType ElementType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGHUDCornerPosition CornerPosition;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGHUDLayer Layer = EPGHUDLayer::GameLayer;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGHUDVisibility DefaultVisibility = EPGHUDVisibility::Visible;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ElementID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UPGHUDBaseWidget> WidgetClass;
};