// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGEquipmentTypes.h"
#include "PGEquipmentItem.h"
#include "Components/SkeletalMeshComponent.h"
#include "PGEquipmentVisualizerComponent.generated.h"

// 장비 염색 변경 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnEquipmentDyeChanged, EPGEquipmentSlot, Slot, EPGDyeChannel, Channel, const FPGDyeInfo&, DyeInfo);

/**
 * 장비 시각화 컴포넌트. 캐릭터의 외형에 장비를 반영합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UPLAYGROUND_API UPGEquipmentVisualizerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPGEquipmentVisualizerComponent();

protected:
    virtual void BeginPlay() override;

    // 캐릭터의 스켈레탈 메시 컴포넌트 참조
    UPROPERTY()
    USkeletalMeshComponent* CharacterMesh;

    // 현재 부착된 장비 메시(정적 메시) 액터들
    UPROPERTY()
    TMap<EPGEquipmentSlot, UStaticMeshComponent*> AttachedStaticMeshes;

    // 현재 부착된 장비 메시(스켈레탈 메시) 컴포넌트들
    UPROPERTY()
    TMap<EPGEquipmentSlot, USkeletalMeshComponent*> AttachedSkeletalMeshes;
    
    // 슬롯별 장비 아이템 참조 저장
    UPROPERTY()
    TMap<EPGEquipmentSlot, UPGEquipmentItem*> EquippedItemRefs;

public:
    // 장비 염색 변경 시 호출되는 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Equipment|Dye")
    FOnEquipmentDyeChanged OnEquipmentDyeChanged;

    /**
     * 특정 슬롯의 장비를 시각적으로 적용
     * @param Slot - 장비 슬롯
     * @param Item - 장비 아이템
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Visual")
    void ApplyEquipmentVisual(EPGEquipmentSlot Slot, UPGEquipmentItem* Item);

    /**
     * 특정 슬롯의 장비를 시각적으로 제거
     * @param Slot - 제거할 장비 슬롯
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Visual")
    void RemoveEquipmentVisual(EPGEquipmentSlot Slot);

    /**
     * 모든 장비를 시각적으로 적용
     * @param EquippedItems - 적용할 장비 아이템 맵
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Visual")
    void ApplyAllEquipmentVisuals(const TMap<EPGEquipmentSlot, UPGEquipmentItem*>& EquippedItems);
    
    /**
     * 장비에 염색을 적용
     * @param Slot - 장비 슬롯
     * @param Channel - 염색 채널
     * @param DyeInfo - 적용할 염색 정보
     * @return 염색 적용 성공 여부
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    bool ApplyDyeToEquipment(EPGEquipmentSlot Slot, EPGDyeChannel Channel, const FPGDyeInfo& DyeInfo);
    
    /**
     * 염색된 장비 시각적으로 업데이트
     * @param Slot - 장비 슬롯
     * @param Channel - 염색 채널
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    void UpdateDyeVisual(EPGEquipmentSlot Slot, EPGDyeChannel Channel);
    
    /**
     * 모든 장비 염색 초기화
     */
    UFUNCTION(BlueprintCallable, Category = "Equipment|Dye")
    void ResetAllDyes();

private:
    /**
     * 스태틱 메시 (무기류) 부착
     * @param Slot - 장비 슬롯
     * @param Item - 장비 아이템
     */
    void AttachStaticMesh(EPGEquipmentSlot Slot, UPGEquipmentItem* Item);

    /**
     * 스켈레탈 메시 (갑옷류) 부착
     * @param Slot - 장비 슬롯
     * @param Item - 장비 아이템
     */
    void AttachSkeletalMesh(EPGEquipmentSlot Slot, UPGEquipmentItem* Item);

    /**
     * 장비에 대한 스켈레탈 메시 컴포넌트 생성
     * @param Item - 장비 아이템
     * @return 생성된 스켈레탈 메시 컴포넌트
     */
    USkeletalMeshComponent* CreateSkeletalMeshComponent(UPGEquipmentItem* Item);
    
    /**
     * 장비에 대한 스태틱 메시 컴포넌트 생성
     * @param Item - 장비 아이템
     * @return 생성된 스태틱 메시 컴포넌트
     */
    UStaticMeshComponent* CreateStaticMeshComponent(UPGEquipmentItem* Item);

    /**
     * 머티리얼 적용
     * @param MeshComponent - 머티리얼을 적용할 메시 컴포넌트
     * @param Materials - 적용할 머티리얼 배열
     */
    void ApplyMaterials(UMeshComponent* MeshComponent, const TArray<TSoftObjectPtr<UMaterialInterface>>& Materials);
    
    /**
     * 염색 적용 함수 (실제 머티리얼 파라미터 설정)
     * @param MeshComponent - 머티리얼이 적용된 메시 컴포넌트
     * @param Item - 장비 아이템
     */
    void ApplyDyesToMeshComponent(UMeshComponent* MeshComponent, UPGEquipmentItem* Item);
    
    /**
     * 특정 염색 채널 적용 함수
     * @param MeshComponent - 머티리얼이 적용된 메시 컴포넌트
     * @param Item - 장비 아이템
     * @param Channel - 염색 채널
     */
    void ApplyDyeChannel(UMeshComponent* MeshComponent, UPGEquipmentItem* Item, EPGDyeChannel Channel);
};

