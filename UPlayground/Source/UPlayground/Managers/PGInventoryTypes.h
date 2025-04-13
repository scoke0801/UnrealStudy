// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGInventoryTypes.generated.h"

/**
 * 인벤토리 결과 열거형
 * 인벤토리 작업의 결과를 나타냅니다.
 */
UENUM(BlueprintType)
enum class EPGInventoryResult : uint8
{
	Success             UMETA(DisplayName = "Success"),
	Failed              UMETA(DisplayName = "Failed"),
	Full                UMETA(DisplayName = "Inventory Full"),
	NotEnoughItems      UMETA(DisplayName = "Not Enough Items"),
	InvalidSlot         UMETA(DisplayName = "Invalid Slot"),
	ItemNotFound        UMETA(DisplayName = "Item Not Found"),
	CannotUse           UMETA(DisplayName = "Cannot Use Item")
};

/**
 * 인벤토리 슬롯 타입 열거형
 * 인벤토리 슬롯의 타입을 나타냅니다.
 */
UENUM(BlueprintType)
enum class EPGInventorySlotType : uint8
{
	Default             UMETA(DisplayName = "Default"),
	Equipment           UMETA(DisplayName = "Equipment"),
	Consumable          UMETA(DisplayName = "Consumable"),
	Quest               UMETA(DisplayName = "Quest"),
	Material            UMETA(DisplayName = "Material")
};

/**
 * 아이템 요구 사항 구조체
 * 퀘스트나 제작 등에 필요한 아이템 요구 사항을 정의합니다.
 */
USTRUCT(BlueprintType)
struct FPGItemRequirement
{
	GENERATED_BODY()

	// 아이템 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Requirement")
	FName ItemID;

	// 필요한 아이템 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Requirement")
	int32 Count;

	// 기본 생성자
	FPGItemRequirement()
		: ItemID(NAME_None)
		, Count(1)
	{
	}

	// 매개변수 생성자
	FPGItemRequirement(FName InItemID, int32 InCount)
		: ItemID(InItemID)
		, Count(InCount)
	{
	}
};
