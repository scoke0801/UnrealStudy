// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGEquipmentTypes.h"
#include "PGEquipmentItem.h"
#include "PGEquipmentComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, EPGEquipmentSlot, Slot, UPGEquipmentItem*, NewItem);

/**
 * 캐릭터의 장비 관리를 담당하는 컴포넌트입니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UPLAYGROUND_API UPGEquipmentComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPGEquipmentComponent();

protected:
    virtual void BeginPlay() override;

public:
    // 현재 장착된 장비 아이템들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
    TMap<EPGEquipmentSlot, UPGEquipmentItem*> EquippedItems;

    // 장비 변경 시 호출되는 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Equipment")
    FOnEquipmentChanged OnEquipmentChanged;

    /**
     * 아이템을 장착합니다.
     * @param Item - 장착할 장비 아이템
     * @return 장착 성공 여부
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool EquipItem(UPGEquipmentItem* Item);

    /**
     * 아이템을 해제합니다.
     * @param Slot - 해제할 장비 슬롯
     * @return 해제 성공 여부
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    bool UnequipItem(EPGEquipmentSlot Slot);

    /**
     * 특정 슬롯의 장착된 아이템을 반환합니다.
     * @param Slot - 조회할 장비 슬롯
     * @return 장착된 아이템 (없으면 nullptr)
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    UPGEquipmentItem* GetEquippedItem(EPGEquipmentSlot Slot) const;

    /**
     * 장비의 통합 스탯을 계산하여 반환합니다.
     * @return 통합된 장비 스탯
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    FPGEquipmentStats CalculateTotalEquipmentStats() const;

/**
     * 장비에 염색 적용
     * @param Slot - 장비 슬롯
     * @param Channel - 염색 채널
     * @param DyeInfo - 염색 정보
     * @return 염색 적용 성공 여부
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    bool ApplyDyeToEquipment(EPGEquipmentSlot Slot, EPGDyeChannel Channel, const FPGDyeInfo& DyeInfo);
    
    /**
     * 장비의 염색 정보 가져오기
     * @param Slot - 장비 슬롯
     * @param Channel - 염색 채널
     * @return 염색 정보
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    FPGDyeInfo GetEquipmentDyeInfo(EPGEquipmentSlot Slot, EPGDyeChannel Channel) const;
    
    /**
     * 장비가 염색 가능한지 확인
     * @param Slot - 장비 슬롯
     * @return 염색 가능 여부
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    bool IsEquipmentDyeable(EPGEquipmentSlot Slot) const;
    
    /**
     * 모든 장비 염색 초기화
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    void ResetAllDyes();

private:
    /**
     * 장비 아이템의 슬롯 호환성 검사
     * @param Item - 검사할 아이템
     * @param Slot - 비교할 슬롯
     * @return 호환성 여부
     */
    bool IsItemCompatibleWithSlot(UPGEquipmentItem* Item, EPGEquipmentSlot Slot) const;

    /**
     * 두손 무기와 한손 무기 간의 충돌 처리
     * @param NewSlot - 새 장비 슬롯
     * @return 충돌 처리 성공 여부
     */
    bool HandleWeaponSlotConflict(EPGEquipmentSlot NewSlot);
};
