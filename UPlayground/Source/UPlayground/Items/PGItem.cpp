// Fill out your copyright notice in the Description page of Project Settings.

#include "PGItem.h"

UPGItem::UPGItem()
	: StackCount(1)
{
}

void UPGItem::Initialize(const FPGItemData& NewItemData, int32 NewStackCount)
{
	ItemData = NewItemData;
	StackCount = FMath::Clamp(NewStackCount, 1, ItemData.MaxStackSize);
}

bool UPGItem::Use(AActor* Character)
{
	// 기본 구현: 아이템 사용 성공 시 스택 감소
	RemoveStack();
	return true;
}

int32 UPGItem::AddStack(int32 Amount)
{
	if (Amount <= 0)
	{
		return 0;
	}

	const int32 SpaceLeft = ItemData.MaxStackSize - StackCount;
	if (SpaceLeft <= 0)
	{
		return 0;
	}

	const int32 ActualAmountToAdd = FMath::Min(Amount, SpaceLeft);
	StackCount += ActualAmountToAdd;
	return ActualAmountToAdd;
}

int32 UPGItem::RemoveStack(int32 Amount)
{
	if (Amount <= 0)
	{
		return 0;
	}

	const int32 ActualAmountToRemove = FMath::Min(Amount, StackCount);
	StackCount -= ActualAmountToRemove;
	return ActualAmountToRemove;
}

bool UPGItem::CanStack(const UPGItem* OtherItem) const
{
	if (OtherItem == nullptr)
	{
		return false;
	}

	// 같은 아이템 ID인지 확인
	return ItemData.ItemID == OtherItem->GetItemID() && StackCount < ItemData.MaxStackSize;
}
