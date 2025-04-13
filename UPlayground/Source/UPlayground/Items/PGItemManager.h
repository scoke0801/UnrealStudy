// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/StreamableManager.h"
#include "PGItem.h"
#include "PGItemManager.generated.h"

// 아이템 업데이트 이벤트 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPGItemsUpdated);

/**
 * 아이템 매니저 클래스
 * 게임 인스턴스 서브시스템으로 구현하여 게임 전체에서 접근 가능
 */
UCLASS(BlueprintType)
class UPLAYGROUND_API UPGItemManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPGItemManager();

	// USubsystem 인터페이스 구현
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/**
	 * 아이템 데이터 테이블 설정 함수
	 * @param NewItemDataTable - 새로운 아이템 데이터 테이블
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Manager")
	void SetItemDataTable(UDataTable* NewItemDataTable);

	/**
	 * 아이템 데이터 가져오기 함수
	 * @param ItemID - 가져올 아이템의 ID
	 * @param OutItemData - 출력 아이템 데이터
	 * @return 아이템 데이터 찾기 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Manager")
	bool GetItemData(FName ItemID, FPGItemData& OutItemData) const;

	/**
	 * 새 아이템 인스턴스 생성 함수
	 * @param ItemID - 생성할 아이템의 ID
	 * @param StackCount - 초기 스택 수
	 * @param OutResult - 생성 성공 여부
	 * @return 생성된 아이템 인스턴스
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Manager")
	UPGItem* CreateItem(FName ItemID, int32 StackCount, bool& OutResult);

	/**
	 * 아이템 타입별 목록 가져오기 함수
	 * @param ItemType - 필터링할 아이템 타입
	 * @return 해당 타입의 아이템 ID 목록
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Manager")
	TArray<FName> GetItemIDsByType(EPGItemType ItemType) const;

	/**
	 * 아이템 희귀도별 목록 가져오기 함수
	 * @param ItemRarity - 필터링할 아이템 희귀도
	 * @return 해당 희귀도의 아이템 ID 목록
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Manager")
	TArray<FName> GetItemIDsByRarity(EPGItemRarity ItemRarity) const;

	/**
	 * 모든 아이템 ID 목록 가져오기 함수
	 * @return 모든 아이템 ID 목록
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Manager")
	TArray<FName> GetAllItemIDs() const;

	/**
	 * 모든 아이템 데이터 목록 가져오기 함수
	 * @return 모든 아이템 데이터 목록
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Manager")
	TArray<FPGItemData> GetAllItemData() const;

	/**
	 * 아이템 아이콘 비동기 로드 함수
	 * @param ItemID - 로드할 아이템의 ID
	 * @param Callback - 로드 완료 시 호출할 델리게이트
	 */
	//UFUNCTION(BlueprintCallable, Category = "Item Manager")
	void LoadItemIcon(FName ItemID, const FStreamableDelegate& Callback);

	/**
	 * 아이템 메쉬 비동기 로드 함수
	 * @param ItemID - 로드할 아이템의 ID
	 * @param Callback - 로드 완료 시 호출할 델리게이트
	 */
	//UFUNCTION(BlueprintCallable, Category = "Item Manager")
	void LoadItemMesh(FName ItemID, const FStreamableDelegate& Callback);

	/**
	 * 아이템 관련 애셋 캐시 해제 함수
	 */
	UFUNCTION(BlueprintCallable, Category = "Item Manager")
	void ReleaseItemAssets();

	// 아이템 업데이트 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Item Manager")
	FOnPGItemsUpdated OnItemsUpdated;

private:
	// 아이템 데이터 테이블
	UPROPERTY()
	UDataTable* ItemDataTable;

	// 아이템 애셋 로드를 위한 스트리머블 매니저 핸들
	TSharedPtr<FStreamableHandle> IconLoadHandle;
	TSharedPtr<FStreamableHandle> MeshLoadHandle;

	// 아이템 ID를 타입별로 캐싱
	TMap<EPGItemType, TArray<FName>> ItemIDsByType;
	
	// 아이템 ID를 희귀도별로 캐싱
	TMap<EPGItemRarity, TArray<FName>> ItemIDsByRarity;

	// 아이템 데이터 캐시 초기화
	void InitializeItemDataCache();

	// 아이템 데이터 캐시 해제
	void ClearItemDataCache();
};
