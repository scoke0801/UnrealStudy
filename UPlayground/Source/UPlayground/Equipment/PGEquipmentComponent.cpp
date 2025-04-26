// Fill out your copyright notice in the Description page of Project Settings.

#include "PGEquipmentComponent.h"

#include "PGEquipmentVisualizerComponent.h"
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

bool UPGEquipmentComponent::ApplyDyeToEquipment(EPGEquipmentSlot Slot, EPGDyeChannel Channel, const FPGDyeInfo& DyeInfo)
{
    // 해당 슬롯에 장비가 있는지 확인
    if (!EquippedItems.Contains(Slot))
    {
        return false;
    }
    
    // 장비 아이템 가져오기
    UPGEquipmentItem* EquipmentItem = EquippedItems[Slot];
    if (!EquipmentItem || !EquipmentItem->IsDyeable())
    {
        return false;
    }
    
    // 시각화 컴포넌트 찾기
    UPGEquipmentVisualizerComponent* VisualizerComp = GetOwner()->FindComponentByClass<UPGEquipmentVisualizerComponent>();
    if (!VisualizerComp)
    {
        // 시각화 컴포넌트가 없으면 아이템에만 염색 적용
        return EquipmentItem->ApplyDye(Channel, DyeInfo);
    }
    
    // 시각화 컴포넌트를 통해 염색 적용
    return VisualizerComp->ApplyDyeToEquipment(Slot, Channel, DyeInfo);
}

FPGDyeInfo UPGEquipmentComponent::GetEquipmentDyeInfo(EPGEquipmentSlot Slot, EPGDyeChannel Channel) const
{
    // 해당 슬롯에 장비가 있는지 확인
    if (!EquippedItems.Contains(Slot))
    {
        return FPGDyeInfo();
    }
    
    // 장비 아이템 가져오기
    UPGEquipmentItem* EquipmentItem = EquippedItems[Slot];
    if (!EquipmentItem)
    {
        return FPGDyeInfo();
    }
    
    // 아이템에서 염색 정보 가져오기
    return EquipmentItem->GetDyeInfo(Channel);
}

bool UPGEquipmentComponent::IsEquipmentDyeable(EPGEquipmentSlot Slot) const
{
    // 해당 슬롯에 장비가 있는지 확인
    if (!EquippedItems.Contains(Slot))
    {
        return false;
    }
    
    // 장비 아이템 가져오기
    UPGEquipmentItem* EquipmentItem = EquippedItems[Slot];
    if (!EquipmentItem)
    {
        return false;
    }
    
    // 염색 가능 여부 확인
    return EquipmentItem->IsDyeable();
}

void UPGEquipmentComponent::ResetAllDyes()
{
    // 시각화 컴포넌트 찾기
    UPGEquipmentVisualizerComponent* VisualizerComp = GetOwner()->FindComponentByClass<UPGEquipmentVisualizerComponent>();
    if (!VisualizerComp)
    {
        // 시각화 컴포넌트가 없으면 각 아이템의 염색만 초기화
        for (auto& ItemPair : EquippedItems)
        {
            UPGEquipmentItem* Item = ItemPair.Value;
            if (Item && Item->IsDyeable())
            {
                Item->ClearAllDyes();
            }
        }
        return;
    }
    
    // 시각화 컴포넌트를 통해 모든 염색 초기화
    VisualizerComp->ResetAllDyes();
}
