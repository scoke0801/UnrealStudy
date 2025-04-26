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
