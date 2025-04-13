// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "PGItem.generated.h"

/**
 * 아이템 타입 열거형
 */
UENUM(BlueprintType)
enum class EPGItemType : uint8
{
	// 기타 아이템
	Misc UMETA(DisplayName = "Misc"),
	// 소비 아이템
	Consumable UMETA(DisplayName = "Consumable"),
	// 장비 아이템
	Equipment UMETA(DisplayName = "Equipment"),
	// 퀘스트 아이템
	Quest UMETA(DisplayName = "Quest")
};

/**
 * 아이템 희귀도 열거형
 */
UENUM(BlueprintType)
enum class EPGItemRarity : uint8
{
	Common UMETA(DisplayName = "Common"),
	Uncommon UMETA(DisplayName = "Uncommon"),
	Rare UMETA(DisplayName = "Rare"),
	Epic UMETA(DisplayName = "Epic"),
	Legendary UMETA(DisplayName = "Legendary")
};

/**
 * 아이템 테이블 구조체
 */
USTRUCT(BlueprintType)
struct FPGItemData : public FTableRowBase
{
	GENERATED_BODY()

	// 아이템 ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemID;

	// 아이템 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Name;

	// 아이템 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Description;

	// 아이템 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EPGItemType ItemType;

	// 아이템 희귀도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	EPGItemRarity Rarity;

	// 아이템 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UTexture2D> Icon;

	// 아이템 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TSoftObjectPtr<UStaticMesh> Mesh;

	// 아이템 최대 스택 크기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 MaxStackSize;

	// 아이템 판매 가격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 SellPrice;

	// 아이템 구매 가격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	int32 BuyPrice;

	// 기본 생성자
	FPGItemData()
		: ItemID(NAME_None)
		, Name(FText::GetEmpty())
		, Description(FText::GetEmpty())
		, ItemType(EPGItemType::Misc)
		, Rarity(EPGItemRarity::Common)
		, MaxStackSize(1)
		, SellPrice(0)
		, BuyPrice(0)
	{
	}
};

/**
 * 아이템 인스턴스 클래스
 */
UCLASS(BlueprintType, Blueprintable)
class UPLAYGROUND_API UPGItem : public UObject
{
	GENERATED_BODY()
	
public:
	UPGItem();

	/**
	 * 아이템 초기화 함수
	 * @param NewItemData - 새로운 아이템 데이터
	 * @param NewStackCount - 초기 스택 수
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	void Initialize(const FPGItemData& NewItemData, int32 NewStackCount = 1);

	/**
	 * 아이템 사용 함수
	 * @param Character - 아이템을 사용하는 캐릭터
	 * @return 아이템 사용 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual bool Use(AActor* Character);

	/**
	 * 스택 추가 함수
	 * @param Amount - 추가할 스택 수
	 * @return 실제로 추가된 스택 수
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 AddStack(int32 Amount = 1);

	/**
	 * 스택 감소 함수
	 * @param Amount - 감소시킬 스택 수
	 * @return 실제로 감소된 스택 수
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	int32 RemoveStack(int32 Amount = 1);

	/**
	 * 아이템이 스택 가능한지 확인하는 함수
	 * @param OtherItem - 비교할 다른 아이템
	 * @return 스택 가능 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	bool CanStack(const UPGItem* OtherItem) const;

	/**
	 * 아이템 ID 반환 함수
	 * @return 아이템 ID
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE FName GetItemID() const { return ItemData.ItemID; }

	/**
	 * 아이템 이름 반환 함수
	 * @return 아이템 이름
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE FText GetName() const { return ItemData.Name; }

	/**
	 * 아이템 설명 반환 함수
	 * @return 아이템 설명
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE FText GetDescription() const { return ItemData.Description; }

	/**
	 * 아이템 타입 반환 함수
	 * @return 아이템 타입
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE EPGItemType GetItemType() const { return ItemData.ItemType; }

	/**
	 * 아이템 희귀도 반환 함수
	 * @return 아이템 희귀도
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE EPGItemRarity GetRarity() const { return ItemData.Rarity; }

	/**
	 * 아이템 아이콘 반환 함수
	 * @return 아이템 아이콘
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE TSoftObjectPtr<UTexture2D> GetIcon() const { return ItemData.Icon; }

	/**
	 * 아이템 메쉬 반환 함수
	 * @return 아이템 메쉬
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE TSoftObjectPtr<UStaticMesh> GetMesh() const { return ItemData.Mesh; }

	/**
	 * 아이템 최대 스택 크기 반환 함수
	 * @return 최대 스택 크기
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE int32 GetMaxStackSize() const { return ItemData.MaxStackSize; }

	/**
	 * 현재 스택 수 반환 함수
	 * @return 현재 스택 수
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE int32 GetStackCount() const { return StackCount; }

	/**
	 * 아이템 판매 가격 반환 함수
	 * @return 판매 가격
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE int32 GetSellPrice() const { return ItemData.SellPrice; }

	/**
	 * 아이템 구매 가격 반환 함수
	 * @return 구매 가격
	 */
	UFUNCTION(BlueprintCallable, Category = "Item")
	FORCEINLINE int32 GetBuyPrice() const { return ItemData.BuyPrice; }

protected:
	// 아이템 데이터
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	FPGItemData ItemData;

	// 현재 스택 수
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	int32 StackCount;
};
