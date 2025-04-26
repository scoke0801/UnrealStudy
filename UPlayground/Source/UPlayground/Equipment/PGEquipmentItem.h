// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Items/PGItem.h"
#include "PGEquipmentTypes.h"
#include "PGEquipmentItem.generated.h"

/**
 * 장비 아이템 클래스. 캐릭터가 착용할 수 있는 장비 아이템을 정의합니다.
 */
UCLASS(Blueprintable)
class UPLAYGROUND_API UPGEquipmentItem : public UPGItem
{
    GENERATED_BODY()

public:
    UPGEquipmentItem();

    /**
     * 장비 초기화 함수
     * @param NewItemData - 새로운 아이템 데이터
     * @param NewEquipmentSlot - 장비 슬롯
     * @param NewEquipmentStats - 장비 스탯
     * @param NewStackCount - 초기 스택 수
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment")
    void InitializeEquipment(const FPGItemData& NewItemData, EPGEquipmentSlot NewEquipmentSlot, const FPGEquipmentStats& NewEquipmentStats, int32 NewStackCount = 1);

    /**
     * 장비 사용 함수 (장착)
     * @param Character - 아이템을 사용하는 캐릭터
     * @return 아이템 사용 성공 여부
     */
    virtual bool Use(AActor* Character) override;

    /**
     * 염색 적용 함수
     * @param Channel - 염색할 채널
     * @param DyeInfo - 적용할 염색 정보
     * @return 염색 적용 성공 여부
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    bool ApplyDye(EPGDyeChannel Channel, const FPGDyeInfo& DyeInfo);

    /**
     * 염색 정보 가져오기
     * @param Channel - 조회할 염색 채널
     * @return 염색 정보
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    FPGDyeInfo GetDyeInfo(EPGDyeChannel Channel) const;

    /**
     * 염색 가능 여부 확인
     * @return 염색 가능 여부
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    bool IsDyeable() const;

    /**
     * 모든 염색 제거 (기본 색상으로 복원)
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    void ClearAllDyes();

public:
    // 장비 슬롯 정보
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
    EPGEquipmentSlot EquipmentSlot;

    // 장비의 스탯 정보
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment")
    FPGEquipmentStats EquipmentStats;

    // 장비 착용 시 적용될 스켈레탈 메시 (갑옷, 헬멧 등)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Visual")
    TSoftObjectPtr<USkeletalMesh> EquipmentMesh;

    // 장비 착용 시 적용될 스태틱 메시 (주로 무기류)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Visual")
    TSoftObjectPtr<UStaticMesh> StaticEquipmentMesh;

    // 무기나 방패가 붙을 소켓 이름 (캐릭터 스켈레톤의 소켓 이름)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Visual")
    FName AttachSocketName;

    // 장비 착용 시 적용될 머티리얼
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Visual")
    TArray<TSoftObjectPtr<UMaterialInterface>> EquipmentMaterials;

    // 염색 가능 여부
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Dye")
    bool bCanBeDyed;

    // 염색 가능한 채널 (장비마다 다를 수 있음)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Dye", meta = (EditCondition = "bCanBeDyed"))
    TArray<EPGDyeChannel> AvailableDyeChannels;

    // 염색 색상 파라미터 이름 (머티리얼 파라미터 이름)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Dye", meta = (EditCondition = "bCanBeDyed"))
    TMap<EPGDyeChannel, FName> DyeParameterNames;

    // 메탈릭 파라미터 이름
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Dye", meta = (EditCondition = "bCanBeDyed"))
    TMap<EPGDyeChannel, FName> MetallicParameterNames;

    // 광택 파라미터 이름
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Equipment|Dye", meta = (EditCondition = "bCanBeDyed"))
    TMap<EPGDyeChannel, FName> RoughnessParameterNames;

protected:
    // 각 채널별 염색 정보
    UPROPERTY()
    TMap<EPGDyeChannel, FPGDyeInfo> AppliedDyes;
};

