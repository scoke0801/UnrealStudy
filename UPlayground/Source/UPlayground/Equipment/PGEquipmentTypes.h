// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PGEquipmentTypes.generated.h"

/**
 * 장비 슬롯 열거형
 */
UENUM(BlueprintType)
enum class EPGEquipmentSlot : uint8
{
    None UMETA(DisplayName = "None"),
    Head UMETA(DisplayName = "Head"),
    Chest UMETA(DisplayName = "Chest"),
    Legs UMETA(DisplayName = "Legs"),
    Feet UMETA(DisplayName = "Feet"),
    Hands UMETA(DisplayName = "Hands"),
    MainHand UMETA(DisplayName = "Main Hand"),
    OffHand UMETA(DisplayName = "Off Hand"),
    TwoHand UMETA(DisplayName = "Two Hand"),
    Accessory UMETA(DisplayName = "Accessory")
};

/**
 * 장비 염색 채널 열거형
 * 장비에서 염색 가능한 부위를 구분
 */
UENUM(BlueprintType)
enum class EPGDyeChannel : uint8
{
    Primary UMETA(DisplayName = "Primary"),
    Secondary UMETA(DisplayName = "Secondary"),
    Tertiary UMETA(DisplayName = "Tertiary"),
    Accent UMETA(DisplayName = "Accent"),
    Metal UMETA(DisplayName = "Metal")
};

/**
 * 장비 염색 정보 구조체
 * 장비에 적용된 색상 정보를 저장
 */
USTRUCT(BlueprintType)
struct FPGDyeInfo
{
    GENERATED_BODY()

    // 색상 정보
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Dye")
    FLinearColor Color = FLinearColor::White;

    // 메탈릭 정도 (0.0 ~ 1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Dye", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Metallic = 0.0f;

    // 광택 정도 (0.0 ~ 1.0)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment|Dye", meta = (ClampMin = "0.0", ClampMax = "1.0"))
    float Roughness = 0.5f;

    // 기본 생성자
    FPGDyeInfo() {}

    // 매개변수가 있는 생성자
    FPGDyeInfo(const FLinearColor& InColor, float InMetallic = 0.0f, float InRoughness = 0.5f)
        : Color(InColor), Metallic(InMetallic), Roughness(InRoughness)
    {}
};

/**
 * 장비 스탯 구조체
 */
USTRUCT(BlueprintType)
struct FPGEquipmentStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    float AttackPower = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    float DefensePower = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    float MagicPower = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equipment")
    float MagicResistance = 0.0f;
};
