// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGItem.h"
#include "PGInventoryComponent.generated.h"

// 인벤토리 아이템 구조체
USTRUCT(BlueprintType)
struct FPGInventorySlot
{
	GENERATED_BODY()

	// 슬롯 인덱스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 SlotIndex;

	// 아이템 인스턴스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	UPGItem* Item;

	// 기본 생성자
	FPGInventorySlot()
		: SlotIndex(-1)
		, Item(nullptr)
	{
	}

	// 매개변수 생성자
	FPGInventorySlot(int32 InSlotIndex, UPGItem* InItem)
		: SlotIndex(InSlotIndex)
		, Item(InItem)
	{
	}
};

// 인벤토리 업데이트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPGInventoryUpdated, int32, SlotIndex, UPGItem*, Item);

// 인벤토리 용량 업데이트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPGInventoryCapacityChanged, int32, NewCapacity);

/**
 * 인벤토리 컴포넌트 클래스
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class UPLAYGROUND_API UPGInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// 생성자
	UPGInventoryComponent();

protected:
	// 컴포넌트 초기화 시 호출
	virtual void BeginPlay() override;
	
	// 게임플레이 시작 시 호출
	virtual void InitializeComponent() override;

public:	
	// 틱 함수
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * 아이템 추가 함수
	 * @param NewItem - 추가할 아이템
	 * @param OutSlotIndex - 추가된 슬롯 인덱스
	 * @param Count - 추가할 아이템 수량
	 * @return 추가 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(UPGItem* NewItem, int32& OutSlotIndex, int32 Count = 1);

	/**
	 * 아이템 제거 함수
	 * @param SlotIndex - 제거할 아이템의 슬롯 인덱스
	 * @param Count - 제거할 아이템 수량
	 * @return 제거 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(int32 SlotIndex, int32 Count = 1);

	/**
	 * 아이템 사용 함수
	 * @param SlotIndex - 사용할 아이템의 슬롯 인덱스
	 * @return 사용 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(int32 SlotIndex);

	/**
	 * 아이템 이동 함수
	 * @param SourceSlotIndex - 이동할 아이템의 원본 슬롯 인덱스
	 * @param DestSlotIndex - 이동할 대상 슬롯 인덱스
	 * @return 이동 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool MoveItem(int32 SourceSlotIndex, int32 DestSlotIndex);

	/**
	 * 슬롯에 아이템이 있는지 확인하는 함수
	 * @param SlotIndex - 확인할 슬롯 인덱스
	 * @return 아이템 존재 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(int32 SlotIndex) const;

	/**
	 * 슬롯의 아이템 가져오기 함수
	 * @param SlotIndex - 가져올 슬롯 인덱스
	 * @return 슬롯의 아이템 (없으면 nullptr)
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UPGItem* GetItem(int32 SlotIndex) const;

	/**
	 * 특정 아이템 ID를 가진 아이템을 찾는 함수
	 * @param ItemID - 찾을 아이템 ID
	 * @param OutSlotIndices - 찾은 아이템의 슬롯 인덱스 배열
	 * @return 아이템 찾기 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool FindItemsByID(FName ItemID, TArray<int32>& OutSlotIndices) const;

	/**
	 * 인벤토리 용량 변경 함수
	 * @param NewCapacity - 새 인벤토리 용량
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetInventoryCapacity(int32 NewCapacity);

	/**
	 * 인벤토리 용량 가져오기 함수
	 * @return 현재 인벤토리 용량
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FORCEINLINE int32 GetInventoryCapacity() const { return InventoryCapacity; }

	/**
	 * 빈 슬롯 수 가져오기 함수
	 * @return 빈 슬롯 수
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetEmptySlotsCount() const;

	/**
	 * 인벤토리 슬롯 배열 가져오기 함수
	 * @return 현재 인벤토리 슬롯 배열
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<FPGInventorySlot> GetInventorySlots() const;

	/**
	 * 인벤토리에 있는 모든 아이템 가져오기 함수
	 * @return 인벤토리의 아이템 배열
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UPGItem*> GetAllItems() const;

	/**
	 * 인벤토리에 있는 특정 타입의 아이템 가져오기 함수
	 * @param ItemType - 필터링할 아이템 타입
	 * @return 해당 타입의 아이템 배열
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UPGItem*> GetItemsByType(EPGItemType ItemType) const;

	/**
	 * 인벤토리에 있는 특정 희귀도의 아이템 가져오기 함수
	 * @param ItemRarity - 필터링할 아이템 희귀도
	 * @return 해당 희귀도의 아이템 배열
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	TArray<UPGItem*> GetItemsByRarity(EPGItemRarity ItemRarity) const;

	/**
	 * 인벤토리 정렬 함수 (ID 기준)
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortInventoryByID();

	/**
	 * 인벤토리 정렬 함수 (타입 기준)
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortInventoryByType();

	/**
	 * 인벤토리 정렬 함수 (희귀도 기준)
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortInventoryByRarity();

	// 인벤토리 업데이트 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnPGInventoryUpdated OnInventoryUpdated;

	// 인벤토리 용량 변경 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnPGInventoryCapacityChanged OnInventoryCapacityChanged;

private:
	// 인벤토리 슬롯 배열
	UPROPERTY()
	TArray<FPGInventorySlot> InventorySlots;

	// 인벤토리 용량
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	int32 InventoryCapacity;

	// 빈 슬롯 찾기 함수
	int32 FindEmptySlot() const;

	// 특정 아이템 ID의 스택 가능한 슬롯 찾기 함수
	int32 FindStackableSlot(UPGItem* Item) const;

	// 슬롯 유효성 검사 함수
	bool IsValidSlot(int32 SlotIndex) const;

	// 인벤토리 슬롯 초기화 함수
	void InitializeInventorySlots();
};
