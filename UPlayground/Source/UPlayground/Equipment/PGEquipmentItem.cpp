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
    
    // 기본적으로 염색 가능으로 설정
    bCanBeDyed = true;
    
    // 기본 염색 채널 추가
    AvailableDyeChannels.Add(EPGDyeChannel::Primary);
    AvailableDyeChannels.Add(EPGDyeChannel::Secondary);
    
    // 기본 파라미터 이름 설정
    DyeParameterNames.Add(EPGDyeChannel::Primary, FName("PrimaryColor"));
    DyeParameterNames.Add(EPGDyeChannel::Secondary, FName("SecondaryColor"));
    MetallicParameterNames.Add(EPGDyeChannel::Primary, FName("PrimaryMetallic"));
    MetallicParameterNames.Add(EPGDyeChannel::Secondary, FName("SecondaryMetallic"));
    RoughnessParameterNames.Add(EPGDyeChannel::Primary, FName("PrimaryRoughness"));
    RoughnessParameterNames.Add(EPGDyeChannel::Secondary, FName("SecondaryRoughness"));
    
    // 기본 염색 정보 설정
    AppliedDyes.Add(EPGDyeChannel::Primary, FPGDyeInfo(FLinearColor::White, 0.0f, 0.5f));
    AppliedDyes.Add(EPGDyeChannel::Secondary, FPGDyeInfo(FLinearColor::White, 0.0f, 0.5f));
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

bool UPGEquipmentItem::ApplyDye(EPGDyeChannel Channel, const FPGDyeInfo& DyeInfo)
{
    // 염색 가능 여부 확인
    if (!IsDyeable())
    {
        return false;
    }
    
    // 해당 채널이 이 장비에서 사용 가능한지 확인
    if (!AvailableDyeChannels.Contains(Channel))
    {
        return false;
    }
    
    // 염색 정보 저장
    AppliedDyes.Add(Channel, DyeInfo);
    
    return true;
}

FPGDyeInfo UPGEquipmentItem::GetDyeInfo(EPGDyeChannel Channel) const
{
    // 해당 채널의 염색 정보 반환
    if (AppliedDyes.Contains(Channel))
    {
        return AppliedDyes[Channel];
    }
    
    // 기본값 반환
    return FPGDyeInfo();
}

bool UPGEquipmentItem::IsDyeable() const
{
    return bCanBeDyed && AvailableDyeChannels.Num() > 0;
}

void UPGEquipmentItem::ClearAllDyes()
{
    // 모든 염색 정보 초기화
    AppliedDyes.Empty();
    
    // 기본 색상으로 채널 재설정
    for (const EPGDyeChannel& Channel : AvailableDyeChannels)
    {
        AppliedDyes.Add(Channel, FPGDyeInfo(FLinearColor::White, 0.0f, 0.5f));
    }
}

