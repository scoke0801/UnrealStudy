// Fill out your copyright notice in the Description page of Project Settings.

#include "PGItemManager.h"
#include "Engine/AssetManager.h"

UPGItemManager::UPGItemManager()
	: ItemDataTable(nullptr)
{
}

void UPGItemManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 기본 초기화 작업
	ClearItemDataCache();
}

void UPGItemManager::Deinitialize()
{
	// 리소스 정리
	ClearItemDataCache();
	ReleaseItemAssets();

	Super::Deinitialize();
}

void UPGItemManager::SetItemDataTable(UDataTable* NewItemDataTable)
{
	if (NewItemDataTable != ItemDataTable)
	{
		ItemDataTable = NewItemDataTable;
		
		// 아이템 데이터 캐시 초기화
		InitializeItemDataCache();
		
		// 아이템 업데이트 이벤트 발생
		OnItemsUpdated.Broadcast();
	}
}

bool UPGItemManager::GetItemData(FName ItemID, FPGItemData& OutItemData) const
{
	if (!ItemDataTable || ItemID.IsNone())
	{
		return false;
	}

	// 데이터 테이블에서 아이템 데이터 조회
	const FPGItemData* FoundItemData = ItemDataTable->FindRow<FPGItemData>(ItemID, TEXT(""), false);
	if (FoundItemData)
	{
		OutItemData = *FoundItemData;
		return true;
	}

	return false;
}

UPGItem* UPGItemManager::CreateItem(FName ItemID, int32 StackCount, bool& OutResult)
{
	OutResult = false;

	if (ItemID.IsNone() || StackCount <= 0)
	{
		return nullptr;
	}

	// 아이템 데이터 조회
	FPGItemData ItemData;
	if (!GetItemData(ItemID, ItemData))
	{
		return nullptr;
	}

	// 아이템 인스턴스 생성
	UPGItem* NewItem = NewObject<UPGItem>();
	if (NewItem)
	{
		NewItem->Initialize(ItemData, StackCount);
		OutResult = true;
	}

	return NewItem;
}

TArray<FName> UPGItemManager::GetItemIDsByType(EPGItemType ItemType) const
{
	// 캐싱된 아이템 타입별 목록 반환
	const TArray<FName>* FoundItems = ItemIDsByType.Find(ItemType);
	if (FoundItems)
	{
		return *FoundItems;
	}

	return TArray<FName>();
}

TArray<FName> UPGItemManager::GetItemIDsByRarity(EPGItemRarity ItemRarity) const
{
	// 캐싱된 아이템 희귀도별 목록 반환
	const TArray<FName>* FoundItems = ItemIDsByRarity.Find(ItemRarity);
	if (FoundItems)
	{
		return *FoundItems;
	}
	
	return TArray<FName>();
}

TArray<FName> UPGItemManager::GetAllItemIDs() const
{
	TArray<FName> Result;

	if (ItemDataTable)
	{
		// UE5.4 호환성을 위해 GetRowMap 사용
		const TMap<FName, uint8*>& RowMap = ItemDataTable->GetRowMap();
		RowMap.GetKeys(Result);
	}

	return Result;
}

TArray<FPGItemData> UPGItemManager::GetAllItemData() const
{
	TArray<FPGItemData> Result;

	if (!ItemDataTable)
	{
		return Result;
	}

	// 모든 아이템 데이터 열거
	TArray<FName> RowNames = GetAllItemIDs();
	for (const FName& RowName : RowNames)
	{
		FPGItemData ItemData;
		if (GetItemData(RowName, ItemData))
		{
			Result.Add(ItemData);
		}
	}

	return Result;
}

void UPGItemManager::LoadItemIcon(FName ItemID, const FStreamableDelegate& Callback)
{
	// 기존 아이콘 로드 핸들 해제
	if (IconLoadHandle.IsValid())
	{
		IconLoadHandle->ReleaseHandle();
		IconLoadHandle.Reset();
	}

	// 아이템 데이터 확인
	FPGItemData ItemData;
	if (!GetItemData(ItemID, ItemData) || !ItemData.Icon.IsValid())
	{
		// 유효하지 않은 경우 즉시 콜백 호출
		if (Callback.IsBound())
		{
			Callback.Execute();
		}
		return;
	}

	// 아이콘 비동기 로드
	IconLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		ItemData.Icon.ToSoftObjectPath(),
		Callback,
		FStreamableManager::AsyncLoadHighPriority
	);
}

void UPGItemManager::LoadItemMesh(FName ItemID, const FStreamableDelegate& Callback)
{
	// 기존 메쉬 로드 핸들 해제
	if (MeshLoadHandle.IsValid())
	{
		MeshLoadHandle->ReleaseHandle();
		MeshLoadHandle.Reset();
	}

	// 아이템 데이터 확인
	FPGItemData ItemData;
	if (!GetItemData(ItemID, ItemData) || !ItemData.Mesh.IsValid())
	{
		// 유효하지 않은 경우 즉시 콜백 호출
		if (Callback.IsBound())
		{
			Callback.Execute();
		}
		return;
	}

	// 메쉬 비동기 로드
	MeshLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		ItemData.Mesh.ToSoftObjectPath(),
		Callback,
		FStreamableManager::AsyncLoadHighPriority
	);
}

void UPGItemManager::ReleaseItemAssets()
{
	// 로드된 애셋 핸들 해제
	if (IconLoadHandle.IsValid())
	{
		IconLoadHandle->ReleaseHandle();
		IconLoadHandle.Reset();
	}

	if (MeshLoadHandle.IsValid())
	{
		MeshLoadHandle->ReleaseHandle();
		MeshLoadHandle.Reset();
	}
}

void UPGItemManager::InitializeItemDataCache()
{
	// 캐시 초기화
	ClearItemDataCache();

	if (!ItemDataTable)
	{
		return;
	}

	// 모든 아이템 데이터를 열거하여 타입별, 희귀도별로 캐싱
	TArray<FName> RowNames = GetAllItemIDs();
	for (const FName& RowName : RowNames)
	{
		FPGItemData ItemData;
		if (GetItemData(RowName, ItemData))
		{
			// 타입별 캐싱
			TArray<FName>& ItemsOfType = ItemIDsByType.FindOrAdd(ItemData.ItemType);
			ItemsOfType.Add(RowName);

			// 희귀도별 캐싱
			TArray<FName>& ItemsOfRarity = ItemIDsByRarity.FindOrAdd(ItemData.Rarity);
			ItemsOfRarity.Add(RowName);
		}
	}
}

void UPGItemManager::ClearItemDataCache()
{
	// 아이템 타입별 캐시 초기화
	ItemIDsByType.Empty();

	// 아이템 희귀도별 캐시 초기화
	ItemIDsByRarity.Empty();
}
