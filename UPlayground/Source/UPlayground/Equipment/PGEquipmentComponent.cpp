// Fill out your copyright notice in the Description page of Project Settings.

#include "PGEquipmentComponent.h"
#include "../Characters/PGBaseCharacter.h"
#include "../Items/PGInventoryComponent.h"

UPGEquipmentComponent::UPGEquipmentComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPGEquipmentComponent::BeginPlay()
{
    Super::BeginPlay();
}

bool UPGEquipmentComponent::EquipItem(UPGEquipmentItem* Item)
{
    if (!Item)
    {
        return false;
    }

    // 장비 슬롯 가져오기
    EPGEquipmentSlot Slot = Item->EquipmentSlot;

    // 아이템 호환성 검사
    if (!IsItemCompatibleWithSlot(Item, Slot))
    {
        return false;
    }

    // 무기 슬롯 충돌 처리
    if ((Slot == EPGEquipmentSlot::MainHand || Slot == EPGEquipmentSlot::OffHand || Slot == EPGEquipmentSlot::TwoHand) &&
        !HandleWeaponSlotConflict(Slot))
    {
        return false;
    }

    // 새 장비 장착
    EquippedItems.Add(Slot, Item);

    // 델리게이트 호출
    OnEquipmentChanged.Broadcast(Slot, Item);

    return true;
}

bool UPGEquipmentComponent::UnequipItem(EPGEquipmentSlot Slot)
{
    if (!EquippedItems.Contains(Slot))
    {
        return false;
    }

    // 인벤토리 컴포넌트 찾기
    UPGInventoryComponent* InventoryComp = GetOwner()->FindComponentByClass<UPGInventoryComponent>();
    if (InventoryComp)
    {
        // 장비 아이템 가져오기
        UPGEquipmentItem* Item = EquippedItems[Slot];
        if (Item)
        {
            // 인벤토리에 아이템 추가 가능한지 확인
            int32 OutSlotIndex = -1;
            if (InventoryComp->AddItem(Item, OutSlotIndex, 1))
            {
                // 장비 슬롯에서 아이템 제거
                EquippedItems.Remove(Slot);
                
                // 델리게이트 호출
                OnEquipmentChanged.Broadcast(Slot, nullptr);
                
                return true;
            }
            // 인벤토리가 가득 차서 아이템을 추가할 수 없음
            return false;
        }
    }
    
    // 인벤토리 컴포넌트가 없는 경우에도 장비는 해제
    EquippedItems.Remove(Slot);
    
    // 델리게이트 호출
    OnEquipmentChanged.Broadcast(Slot, nullptr);

    return true;
}

UPGEquipmentItem* UPGEquipmentComponent::GetEquippedItem(EPGEquipmentSlot Slot) const
{
    if (EquippedItems.Contains(Slot))
    {
        return EquippedItems[Slot];
    }
    return nullptr;
}

FPGEquipmentStats UPGEquipmentComponent::CalculateTotalEquipmentStats() const
{
    FPGEquipmentStats TotalStats;

    // 모든 장착된 장비의 스탯을 누적
    for (const auto& EquippedItem : EquippedItems)
    {
        if (EquippedItem.Value)
        {
            TotalStats.AttackPower += EquippedItem.Value->EquipmentStats.AttackPower;
            TotalStats.DefensePower += EquippedItem.Value->EquipmentStats.DefensePower;
            TotalStats.MagicPower += EquippedItem.Value->EquipmentStats.MagicPower;
            TotalStats.MagicResistance += EquippedItem.Value->EquipmentStats.MagicResistance;
        }
    }

    return TotalStats;
}

bool UPGEquipmentComponent::IsItemCompatibleWithSlot(UPGEquipmentItem* Item, EPGEquipmentSlot Slot) const
{
    if (!Item)
    {
        return false;
    }

    return Item->EquipmentSlot == Slot;
}

bool UPGEquipmentComponent::HandleWeaponSlotConflict(EPGEquipmentSlot NewSlot)
{
    // 두손 무기를 장착하려는 경우, 주무기와 부무기를 제거
    if (NewSlot == EPGEquipmentSlot::TwoHand)
    {
        if (EquippedItems.Contains(EPGEquipmentSlot::MainHand))
        {
            UnequipItem(EPGEquipmentSlot::MainHand);
        }
        if (EquippedItems.Contains(EPGEquipmentSlot::OffHand))
        {
            UnequipItem(EPGEquipmentSlot::OffHand);
        }
    }
    // 주무기나 부무기를 장착하려는 경우, 두손 무기를 제거
    else if (NewSlot == EPGEquipmentSlot::MainHand || NewSlot == EPGEquipmentSlot::OffHand)
    {
        if (EquippedItems.Contains(EPGEquipmentSlot::TwoHand))
        {
            UnequipItem(EPGEquipmentSlot::TwoHand);
        }
    }

    return true;
}
