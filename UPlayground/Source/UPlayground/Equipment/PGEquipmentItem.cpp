// Fill out your copyright notice in the Description page of Project Settings.

#include "PGEquipmentItem.h"
#include "../Characters/PGBaseCharacter.h"
#include "PGEquipmentComponent.h"

UPGEquipmentItem::UPGEquipmentItem()
{
    // 기본적으로 장비 타입으로 설정
    // ItemType은 직접 멤버가 아니라 ItemData 내부에 있음
    ItemData.ItemType = EPGItemType::Equipment;
    EquipmentSlot = EPGEquipmentSlot::None;
    AttachSocketName = NAME_None;
}

void UPGEquipmentItem::InitializeEquipment(const FPGItemData& NewItemData, EPGEquipmentSlot NewEquipmentSlot, 
                                          const FPGEquipmentStats& NewEquipmentStats, int32 NewStackCount)
{
    // 부모 클래스의 초기화 함수 호출
    Initialize(NewItemData, NewStackCount);
    
    // 장비 관련 속성 설정
    EquipmentSlot = NewEquipmentSlot;
    EquipmentStats = NewEquipmentStats;
}

bool UPGEquipmentItem::Use(AActor* Character)
{
    // 캐릭터 확인
    APGBaseCharacter* BaseCharacter = Cast<APGBaseCharacter>(Character);
    if (!BaseCharacter)
    {
        return false;
    }

    // 캐릭터의 장비 컴포넌트 가져오기
    UPGEquipmentComponent* EquipmentComp = BaseCharacter->FindComponentByClass<UPGEquipmentComponent>();
    if (!EquipmentComp)
    {
        return false;
    }

    // 장비 장착 시도
    bool bEquipped = EquipmentComp->EquipItem(this);
    
    // 장착 성공 시 아이템 스택 감소
    if (bEquipped)
    {
        // 장비는 장착 시 인벤토리에서 제거되므로 여기서는 스택을 감소시키지 않음
        // RemoveStack() 호출 없음
        return true;
    }
    
    return false;
}
