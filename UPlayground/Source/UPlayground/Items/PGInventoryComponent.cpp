// Fill out your copyright notice in the Description page of Project Settings.

#include "PGInventoryComponent.h"
#include "GameFramework/Actor.h"

// 생성자
UPGInventoryComponent::UPGInventoryComponent()
{
	// 기본값 설정
	PrimaryComponentTick.bCanEverTick = false;
	InventoryCapacity = 20; // 기본 인벤토리 용량
}

void UPGInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 인벤토리 초기화
	InitializeInventorySlots();
}

void UPGInventoryComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// 추가 초기화 작업이 필요한 경우 여기에 구현
}

void UPGInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 틱에서 수행할 작업이 있다면 여기에 구현
}

bool UPGInventoryComponent::AddItem(UPGItem* NewItem, int32& OutSlotIndex, int32 Count)
{
	if (!NewItem || Count <= 0)
	{
		OutSlotIndex = -1;
		return false;
	}

	// 스택 가능한 동일 아이템이 있는지 확인
	int32 StackableSlotIndex = FindStackableSlot(NewItem);
	if (StackableSlotIndex != -1)
	{
		// 동일 아이템이 이미 있는 경우 스택 추가
		UPGItem* ExistingItem = InventorySlots[StackableSlotIndex].Item;
		int32 AddedCount = ExistingItem->AddStack(Count);
		
		// 성공적으로 스택 추가한 경우
		if (AddedCount > 0)
		{
			OutSlotIndex = StackableSlotIndex;
			OnInventoryUpdated.Broadcast(StackableSlotIndex, ExistingItem);
			return true;
		}
	}

	// 빈 슬롯을 찾아 새로운 아이템 추가
	int32 EmptySlotIndex = FindEmptySlot();
	if (EmptySlotIndex != -1)
	{
		// 빈 슬롯에 새 아이템 추가
		InventorySlots[EmptySlotIndex].Item = NewItem;
		NewItem->AddStack(Count - 1); // 기본 1개에 추가 수량
		
		OutSlotIndex = EmptySlotIndex;
		OnInventoryUpdated.Broadcast(EmptySlotIndex, NewItem);
		return true;
	}

	// 인벤토리가 가득 찬 경우
	OutSlotIndex = -1;
	return false;
}

bool UPGInventoryComponent::RemoveItem(int32 SlotIndex, int32 Count)
{
	if (!IsValidSlot(SlotIndex) || Count <= 0)
	{
		return false;
	}

	UPGItem* Item = InventorySlots[SlotIndex].Item;
	if (!Item)
	{
		return false;
	}

	// 아이템 스택 감소
	int32 RemovedCount = Item->RemoveStack(Count);
	
	// 스택이 0이 되면 아이템 제거
	if (Item->GetStackCount() <= 0)
	{
		InventorySlots[SlotIndex].Item = nullptr;
		OnInventoryUpdated.Broadcast(SlotIndex, nullptr);
	}
	else
	{
		OnInventoryUpdated.Broadcast(SlotIndex, Item);
	}

	return RemovedCount > 0;
}

bool UPGInventoryComponent::UseItem(int32 SlotIndex)
{
	if (!IsValidSlot(SlotIndex))
	{
		return false;
	}

	UPGItem* Item = InventorySlots[SlotIndex].Item;
	if (!Item)
	{
		return false;
	}

	// 아이템 사용
	bool bUsed = Item->Use(GetOwner());
	
	// 사용 후 스택 감소
	if (bUsed)
	{
		RemoveItem(SlotIndex, 1);
	}

	return bUsed;
}

bool UPGInventoryComponent::MoveItem(int32 SourceSlotIndex, int32 DestSlotIndex)
{
	if (!IsValidSlot(SourceSlotIndex) || !IsValidSlot(DestSlotIndex) || SourceSlotIndex == DestSlotIndex)
	{
		return false;
	}

	UPGItem* SourceItem = InventorySlots[SourceSlotIndex].Item;
	UPGItem* DestItem = InventorySlots[DestSlotIndex].Item;

	if (!SourceItem)
	{
		return false;
	}

	// 대상 슬롯이 비어있는 경우
	if (!DestItem)
	{
		InventorySlots[DestSlotIndex].Item = SourceItem;
		InventorySlots[SourceSlotIndex].Item = nullptr;
		
		// 인벤토리 업데이트 이벤트 발생
		OnInventoryUpdated.Broadcast(SourceSlotIndex, nullptr);
		OnInventoryUpdated.Broadcast(DestSlotIndex, SourceItem);
		
		return true;
	}

	// 동일한 아이템이고 스택 가능한 경우
	if (SourceItem->CanStack(DestItem))
	{
		int32 AddedCount = DestItem->AddStack(SourceItem->GetStackCount());
		
		// 모든 스택을 이동한 경우
		if (AddedCount == SourceItem->GetStackCount())
		{
			InventorySlots[SourceSlotIndex].Item = nullptr;
			OnInventoryUpdated.Broadcast(SourceSlotIndex, nullptr);
			OnInventoryUpdated.Broadcast(DestSlotIndex, DestItem);
			return true;
		}
		// 일부 스택만 이동한 경우
		else if (AddedCount > 0)
		{
			SourceItem->RemoveStack(AddedCount);
			OnInventoryUpdated.Broadcast(SourceSlotIndex, SourceItem);
			OnInventoryUpdated.Broadcast(DestSlotIndex, DestItem);
			return true;
		}
	}

	// 서로 다른 아이템인 경우 위치 교환
	InventorySlots[SourceSlotIndex].Item = DestItem;
	InventorySlots[DestSlotIndex].Item = SourceItem;
	
	// 인벤토리 업데이트 이벤트 발생
	OnInventoryUpdated.Broadcast(SourceSlotIndex, DestItem);
	OnInventoryUpdated.Broadcast(DestSlotIndex, SourceItem);
	
	return true;
}

bool UPGInventoryComponent::HasItem(int32 SlotIndex) const
{
	return IsValidSlot(SlotIndex) && InventorySlots[SlotIndex].Item != nullptr;
}

UPGItem* UPGInventoryComponent::GetItem(int32 SlotIndex) const
{
	if (!IsValidSlot(SlotIndex))
	{
		return nullptr;
	}

	return InventorySlots[SlotIndex].Item;
}

bool UPGInventoryComponent::FindItemsByID(FName ItemID, TArray<int32>& OutSlotIndices) const
{
	OutSlotIndices.Empty();

	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (InventorySlots[i].Item && InventorySlots[i].Item->GetItemID() == ItemID)
		{
			OutSlotIndices.Add(i);
		}
	}

	return OutSlotIndices.Num() > 0;
}

void UPGInventoryComponent::SetInventoryCapacity(int32 NewCapacity)
{
	if (NewCapacity <= 0 || NewCapacity == InventoryCapacity)
	{
		return;
	}

	int32 OldCapacity = InventoryCapacity;
	InventoryCapacity = NewCapacity;

	// 크기가 증가하면 새 슬롯 추가
	if (NewCapacity > OldCapacity)
	{
		for (int32 i = OldCapacity; i < NewCapacity; ++i)
		{
			InventorySlots.Add(FPGInventorySlot(i, nullptr));
		}
	}
	// 크기가 감소하면 아이템 정리
	else if (NewCapacity < OldCapacity)
	{
		// 사라질 슬롯에 있는 아이템 처리
		for (int32 i = NewCapacity; i < OldCapacity; ++i)
		{
			UPGItem* Item = InventorySlots[i].Item;
			if (Item)
			{
				// 빈 슬롯을 찾아 이동 시도
				int32 EmptySlot = FindEmptySlot();
				if (EmptySlot != -1)
				{
					InventorySlots[EmptySlot].Item = Item;
					OnInventoryUpdated.Broadcast(EmptySlot, Item);
				}
				// 아이템 드롭 또는 파괴 처리
				else
				{
					// 아이템 드롭 로직 구현 (또는 간단히 파괴)
				}
			}
		}

		// 슬롯 배열 크기 조정
		InventorySlots.SetNum(NewCapacity);
	}

	// 용량 변경 이벤트 발생
	OnInventoryCapacityChanged.Broadcast(NewCapacity);
}

int32 UPGInventoryComponent::GetEmptySlotsCount() const
{
	int32 EmptyCount = 0;
	for (const FPGInventorySlot& Slot : InventorySlots)
	{
		if (!Slot.Item)
		{
			EmptyCount++;
		}
	}
	return EmptyCount;
}

TArray<FPGInventorySlot> UPGInventoryComponent::GetInventorySlots() const
{
	return InventorySlots;
}

TArray<UPGItem*> UPGInventoryComponent::GetAllItems() const
{
	TArray<UPGItem*> Items;
	for (const FPGInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item)
		{
			Items.Add(Slot.Item);
		}
	}
	return Items;
}

TArray<UPGItem*> UPGInventoryComponent::GetItemsByType(EPGItemType ItemType) const
{
	TArray<UPGItem*> Items;
	for (const FPGInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item && Slot.Item->GetItemType() == ItemType)
		{
			Items.Add(Slot.Item);
		}
	}
	return Items;
}

TArray<UPGItem*> UPGInventoryComponent::GetItemsByRarity(EPGItemRarity ItemRarity) const
{
	TArray<UPGItem*> Items;
	for (const FPGInventorySlot& Slot : InventorySlots)
	{
		if (Slot.Item && Slot.Item->GetRarity() == ItemRarity)
		{
			Items.Add(Slot.Item);
		}
	}
	return Items;
}

void UPGInventoryComponent::SortInventoryByID()
{
	// 정렬 로직 구현
	TArray<UPGItem*> Items = GetAllItems();
	Items.Sort([](const UPGItem& A, const UPGItem& B) {
		return A.GetItemID().ToString() < B.GetItemID().ToString();
	});

	// 정렬된 아이템 다시 배치
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		InventorySlots[i].Item = nullptr;
	}

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		InventorySlots[i].Item = Items[i];
		OnInventoryUpdated.Broadcast(i, Items[i]);
	}
}

void UPGInventoryComponent::SortInventoryByType()
{
	// 정렬 로직 구현
	TArray<UPGItem*> Items = GetAllItems();
	Items.Sort([](const UPGItem& A, const UPGItem& B) {
		return (int32)A.GetItemType() < (int32)B.GetItemType();
	});

	// 정렬된 아이템 다시 배치
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		InventorySlots[i].Item = nullptr;
	}

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		InventorySlots[i].Item = Items[i];
		OnInventoryUpdated.Broadcast(i, Items[i]);
	}
}

void UPGInventoryComponent::SortInventoryByRarity()
{
	// 정렬 로직 구현
	TArray<UPGItem*> Items = GetAllItems();
	Items.Sort([](const UPGItem& A, const UPGItem& B) {
		return (int32)A.GetRarity() > (int32)B.GetRarity(); // 희귀도는 높은 것이 먼저 오도록
	});

	// 정렬된 아이템 다시 배치
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		InventorySlots[i].Item = nullptr;
	}

	for (int32 i = 0; i < Items.Num(); ++i)
	{
		InventorySlots[i].Item = Items[i];
		OnInventoryUpdated.Broadcast(i, Items[i]);
	}
}

// 비어있는 슬롯 찾기
int32 UPGInventoryComponent::FindEmptySlot() const
{
	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		if (!InventorySlots[i].Item)
		{
			return i;
		}
	}
	return -1;
}

// 스택 가능한 슬롯 찾기
int32 UPGInventoryComponent::FindStackableSlot(UPGItem* Item) const
{
	if (!Item)
	{
		return -1;
	}

	for (int32 i = 0; i < InventorySlots.Num(); ++i)
	{
		UPGItem* SlotItem = InventorySlots[i].Item;
		if (SlotItem && Item->CanStack(SlotItem) && SlotItem->GetStackCount() < SlotItem->GetMaxStackSize())
		{
			return i;
		}
	}
	return -1;
}

// 슬롯 유효성 확인
bool UPGInventoryComponent::IsValidSlot(int32 SlotIndex) const
{
	return SlotIndex >= 0 && SlotIndex < InventorySlots.Num();
}

// 인벤토리 슬롯 초기화
void UPGInventoryComponent::InitializeInventorySlots()
{
	InventorySlots.Empty(InventoryCapacity);
	
	for (int32 i = 0; i < InventoryCapacity; ++i)
	{
		InventorySlots.Add(FPGInventorySlot(i, nullptr));
	}
}
