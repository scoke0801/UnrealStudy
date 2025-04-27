// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGBaseManager.h"
#include "PGInventoryTypes.h"
#include "PGSaveTypes.h"
#include "PGInventoryManager.generated.h"

/**
 * 인벤토리 매니저
 * 플레이어의 인벤토리와 아이템 관리를 담당합니다.
 */
UCLASS(Blueprintable)
class UPLAYGROUND_API UPGInventoryManager : public UPGBaseManager, public IPGSavableInterface
{
	GENERATED_BODY()

public:
	UPGInventoryManager();

	//~Begin UPGBaseManager interface
	virtual bool OnInitialize_Implementation() override;
	virtual void OnShutdown_Implementation() override;
	virtual void OnUpdate_Implementation(float DeltaTime) override;
	virtual void OnGameStateChange_Implementation(EPGGameStateType NewState) override;
	//~End UPGBaseManager interface

	/**
	 * 아이템 추가
	 * @param ItemID 추가할 아이템 ID
	 * @param Count 추가할 아이템 수량
	 * @return 추가 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(FName ItemID, int32 Count = 1);

	/**
	 * 아이템 제거
	 * @param ItemID 제거할 아이템 ID
	 * @param Count 제거할 아이템 수량
	 * @return 제거 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemID, int32 Count = 1);

	/**
	 * 아이템 소유 여부 확인
	 * @param ItemID 확인할 아이템 ID
	 * @param Count 필요한 아이템 수량
	 * @return 조건 충족 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool HasItem(FName ItemID, int32 Count = 1) const;

	/**
	 * 아이템 수량 획득
	 * @param ItemID 확인할 아이템 ID
	 * @return 보유한 아이템 수량
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetItemCount(FName ItemID) const;

	/**
	 * 인벤토리 용량 확인
	 * @return 현재 사용 중인 인벤토리 슬롯 수
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetCurrentCapacity() const;

	/**
	 * 인벤토리 최대 용량 확인
	 * @return 최대 인벤토리 슬롯 수
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 GetMaxCapacity() const;

	/**
	 * 인벤토리 최대 용량 설정
	 * @param NewCapacity 새 최대 용량
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetMaxCapacity(int32 NewCapacity);

	/**
	 * 인벤토리 변경 이벤트에 대한 델리게이트
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPGInventoryChangedSignature, FName, ItemID, int32, NewCount, int32, Delta);
	
	/**
	 * 인벤토리 변경 이벤트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnPGInventoryChangedSignature OnInventoryChanged;

	/**
	 * 인벤토리 용량 변경 이벤트에 대한 델리게이트
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPGCapacityChangedSignature, int32, NewCapacity, int32, OldCapacity);
	
	/**
	 * 인벤토리 용량 변경 이벤트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnPGCapacityChangedSignature OnCapacityChanged;

	//~Begin IPGSavableInterface
	virtual bool SaveData_Implementation(class UPGSaveGame* SaveGame) override;
	virtual bool LoadData_Implementation(class UPGSaveGame* SaveGame) override;
	//~End IPGSavableInterface

protected:
	/** 인벤토리 아이템 맵 (아이템 ID -> 수량) */
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TMap<FName, int32> Items;

	/** 인벤토리 최대 용량 */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 MaxCapacity;

	/** 자동 저장 타이머 */
	float SaveTimer;

	/** 자동 저장 간격 (초) */
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float AutoSaveInterval;

	/**
	 * 인벤토리 데이터 저장
	 * @return 저장 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool SaveInventory();

	/**
	 * 인벤토리 데이터 로드
	 * @return 로드 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool LoadInventory();
};
