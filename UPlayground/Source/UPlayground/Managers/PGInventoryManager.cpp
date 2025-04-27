// Copyright (c) 2025. All Rights Reserved.

#include "PGInventoryManager.h"
#include "PGSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

UPGInventoryManager::UPGInventoryManager()
	: MaxCapacity(50)
	, SaveTimer(0.0f)
	, AutoSaveInterval(300.0f) // 5분
{
	ManagerName = FName("InventoryManager");
	
	// 인벤토리 매니저는 인게임 상태와 인벤토리 상태에서 활성화
	ReactToGameStates = {
		EPGGameStateType::GameRunning,
		EPGGameStateType::Inventory
	};
}

bool UPGInventoryManager::OnInitialize_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 초기화 중..."));
	
	// 인벤토리 데이터 로드
	if (!LoadInventory())
	{
		UE_LOG(LogTemp, Warning, TEXT("[InventoryManager] 인벤토리 로드 실패, 기본 인벤토리 사용"));
	}
	
	return true;
}

void UPGInventoryManager::OnShutdown_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 종료 중..."));
	
	// 인벤토리 데이터 저장
	if (!SaveInventory())
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryManager] 인벤토리 저장 실패"));
	}
}

void UPGInventoryManager::OnUpdate_Implementation(float DeltaTime)
{
	// 자동 저장 타이머 업데이트
	if (AutoSaveInterval > 0.0f)
	{
		SaveTimer += DeltaTime;
		if (SaveTimer >= AutoSaveInterval)
		{
			SaveTimer = 0.0f;
			SaveInventory();
		}
	}
}

void UPGInventoryManager::OnGameStateChange_Implementation(EPGGameStateType NewState)
{
	if (NewState == EPGGameStateType::Inventory)
	{
		UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 인벤토리 UI 열림"));
	}
	else if (CurrentGameState == EPGGameStateType::Inventory && NewState != EPGGameStateType::Inventory)
	{
		UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 인벤토리 UI 닫힘"));
		
		// 인벤토리가 닫힐 때 데이터 저장
		SaveInventory();
	}
}

bool UPGInventoryManager::AddItem(FName ItemID, int32 Count)
{
	if (ItemID == NAME_None || Count <= 0)
	{
		return false;
	}
	
	int32 CurrentCount = GetItemCount(ItemID);
	int32 NewCount = CurrentCount + Count;
	
	Items.Add(ItemID, NewCount);
	
	// 델리게이트 브로드캐스트
	OnInventoryChanged.Broadcast(ItemID, NewCount, Count);
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 아이템 추가: %s, 수량: %d, 신규 수량: %d"), 
		*ItemID.ToString(), Count, NewCount);
	
	return true;
}

bool UPGInventoryManager::RemoveItem(FName ItemID, int32 Count)
{
	if (ItemID == NAME_None || Count <= 0)
	{
		return false;
	}
	
	int32 CurrentCount = GetItemCount(ItemID);
	if (CurrentCount < Count)
	{
		// 아이템이 충분하지 않음
		return false;
	}
	
	int32 NewCount = CurrentCount - Count;
	if (NewCount > 0)
	{
		Items.Add(ItemID, NewCount);
	}
	else
	{
		Items.Remove(ItemID);
	}
	
	// 델리게이트 브로드캐스트
	OnInventoryChanged.Broadcast(ItemID, NewCount, -Count);
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 아이템 제거: %s, 수량: %d, 신규 수량: %d"), 
		*ItemID.ToString(), Count, NewCount);
	
	return true;
}

bool UPGInventoryManager::HasItem(FName ItemID, int32 Count) const
{
	if (ItemID == NAME_None || Count <= 0)
	{
		return false;
	}
	
	int32 CurrentCount = GetItemCount(ItemID);
	return CurrentCount >= Count;
}

int32 UPGInventoryManager::GetItemCount(FName ItemID) const
{
	if (ItemID == NAME_None)
	{
		return 0;
	}
	
	const int32* CountPtr = Items.Find(ItemID);
	return CountPtr ? *CountPtr : 0;
}

int32 UPGInventoryManager::GetCurrentCapacity() const
{
	return Items.Num();
}

int32 UPGInventoryManager::GetMaxCapacity() const
{
	return MaxCapacity;
}

void UPGInventoryManager::SetMaxCapacity(int32 NewCapacity)
{
	if (NewCapacity < 0)
	{
		return;
	}
	
	int32 OldCapacity = MaxCapacity;
	MaxCapacity = NewCapacity;
	
	// 델리게이트 브로드캐스트
	OnCapacityChanged.Broadcast(NewCapacity, OldCapacity);
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 인벤토리 용량 변경: %d -> %d"), OldCapacity, NewCapacity);
}

bool UPGInventoryManager::SaveInventory()
{
	// 저장 시스템 예시 (실제 구현은 게임의 저장 시스템에 따라 달라질 수 있음)
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 인벤토리 저장 중..."));
	
	// 실제 구현 시에는 USaveGame 클래스를 활용한 저장 시스템 구현
	return true;
}

bool UPGInventoryManager::LoadInventory()
{
	// 로드 시스템 예시 (실제 구현은 게임의 저장 시스템에 따라 달라질 수 있음)
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 인벤토리 로드 중..."));
	
	// 실제 구현 시에는 USaveGame 클래스를 활용한 로드 시스템 구현
	
	// 테스트용 초기 인벤토리 아이템 설정
	Items.Add(FName("Weapon_Sword"), 1);
	Items.Add(FName("Potion_Health"), 5);
	Items.Add(FName("Material_Wood"), 20);
	
	return true;
}

bool UPGInventoryManager::SaveData_Implementation(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	// 인벤토리 아이템 데이터 저장
	SaveGame->InventoryItems = Items;
	
	// 인벤토리 용량 저장
	SaveGame->InventoryCapacity = MaxCapacity;
	
	// 장착 아이템 데이터는 플레이어 캐릭터의 장비 컴포넌트에서 가져와야 함
	// 현재는 생략
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 인벤토리 데이터 저장 완료"));
	return true;
}

bool UPGInventoryManager::LoadData_Implementation(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	// 인벤토리 아이템 데이터 로드
	Items = SaveGame->InventoryItems;
	
	// 인벤토리 용량 로드
	if (SaveGame->InventoryCapacity > 0)
	{
		SetMaxCapacity(SaveGame->InventoryCapacity);
	}
	
	// 모든 아이템에 대해 변경 이벤트 브로드캐스트
	for (const auto& Item : Items)
	{
		OnInventoryChanged.Broadcast(Item.Key, Item.Value, Item.Value);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[InventoryManager] 인벤토리 데이터 로드 완료"));
	return true;
}
