// Fill out your copyright notice in the Description page of Project Settings.

#include "PGEquipmentVisualizerComponent.h"
#include "GameFramework/Character.h"
#include "PGEquipmentComponent.h"

UPGEquipmentVisualizerComponent::UPGEquipmentVisualizerComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UPGEquipmentVisualizerComponent::BeginPlay()
{
    Super::BeginPlay();

    // 캐릭터의 스켈레탈 메시 컴포넌트 찾기
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (Character)
    {
        CharacterMesh = Character->GetMesh();

        // 장비 컴포넌트 찾기 및 이벤트 바인딩
        UPGEquipmentComponent* EquipmentComp = Character->FindComponentByClass<UPGEquipmentComponent>();
        if (EquipmentComp)
        {
            EquipmentComp->OnEquipmentChanged.AddDynamic(this, &UPGEquipmentVisualizerComponent::ApplyEquipmentVisual);
        }
    }
}

void UPGEquipmentVisualizerComponent::ApplyEquipmentVisual(EPGEquipmentSlot Slot, UPGEquipmentItem* Item)
{
    // 기존 장비 시각 효과 제거
    RemoveEquipmentVisual(Slot);

    if (!Item || !CharacterMesh)
    {
        return;
    }

    // 아이템 타입에 따라 적절한 시각화 방법 선택
    if (!Item->StaticEquipmentMesh.IsNull())
    {
        // 스태틱 메시(주로 무기) 부착
        AttachStaticMesh(Slot, Item);
    }
    else if (!Item->EquipmentMesh.IsNull())
    {
        // 스켈레탈 메시(주로 갑옷) 부착
        AttachSkeletalMesh(Slot, Item);
    }
}

void UPGEquipmentVisualizerComponent::RemoveEquipmentVisual(EPGEquipmentSlot Slot)
{
    // 스태틱 메시 제거
    if (AttachedStaticMeshes.Contains(Slot))
    {
        UStaticMeshComponent* MeshComp = AttachedStaticMeshes[Slot];
        if (MeshComp)
        {
            MeshComp->DestroyComponent();
        }
        AttachedStaticMeshes.Remove(Slot);
    }

    // 스켈레탈 메시 제거
    if (AttachedSkeletalMeshes.Contains(Slot))
    {
        USkeletalMeshComponent* MeshComp = AttachedSkeletalMeshes[Slot];
        if (MeshComp)
        {
            MeshComp->DestroyComponent();
        }
        AttachedSkeletalMeshes.Remove(Slot);
    }
}

void UPGEquipmentVisualizerComponent::ApplyAllEquipmentVisuals(const TMap<EPGEquipmentSlot, UPGEquipmentItem*>& EquippedItems)
{
    // 모든 장비 시각 효과 제거
    for (auto& SlotItem : AttachedSkeletalMeshes)
    {
        RemoveEquipmentVisual(SlotItem.Key);
    }
    
    for (auto& SlotItem : AttachedStaticMeshes)
    {
        RemoveEquipmentVisual(SlotItem.Key);
    }

    // 모든 장착된 장비에 대해 시각 효과 적용
    for (const auto& ItemPair : EquippedItems)
    {
        ApplyEquipmentVisual(ItemPair.Key, ItemPair.Value);
    }
}

void UPGEquipmentVisualizerComponent::AttachStaticMesh(EPGEquipmentSlot Slot, UPGEquipmentItem* Item)
{
    if (!Item || !CharacterMesh || Item->StaticEquipmentMesh.IsNull())
    {
        return;
    }

    // 메시 컴포넌트 생성
    UStaticMeshComponent* MeshComp = CreateStaticMeshComponent(Item);
    if (!MeshComp)
    {
        return;
    }

    // 소켓 이름 결정
    FName SocketName = Item->AttachSocketName;
    if (SocketName.IsNone())
    {
        // 소켓 이름이 지정되지 않은 경우 기본 소켓 이름 사용
        switch (Slot)
        {
            case EPGEquipmentSlot::MainHand:
                SocketName = FName("weapon_r");
                break;
            case EPGEquipmentSlot::OffHand:
                SocketName = FName("weapon_l");
                break;
            case EPGEquipmentSlot::TwoHand:
                SocketName = FName("weapon_r");
                break;
            default:
                // 다른 슬롯의 경우 캐릭터 루트에 부착
                break;
        }
    }

    // 소켓이 존재하는지 확인
    if (!SocketName.IsNone() && CharacterMesh->DoesSocketExist(SocketName))
    {
        // 메시를 소켓에 부착
        MeshComp->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
    }
    else
    {
        // 소켓이 없는 경우 캐릭터 메시에 직접 부착
        MeshComp->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    }

    // 머티리얼 적용
    ApplyMaterials(MeshComp, Item->EquipmentMaterials);

    // 컴포넌트 등록
    AttachedStaticMeshes.Add(Slot, MeshComp);
}

void UPGEquipmentVisualizerComponent::AttachSkeletalMesh(EPGEquipmentSlot Slot, UPGEquipmentItem* Item)
{
    if (!Item || !CharacterMesh || Item->EquipmentMesh.IsNull())
    {
        return;
    }

    // 메시 컴포넌트 생성
    USkeletalMeshComponent* MeshComp = CreateSkeletalMeshComponent(Item);
    if (!MeshComp)
    {
        return;
    }

    // 마스터 포즈 컴포넌트로 캐릭터 메시 설정 (UE5.4에서 추천하는 방식으로 변경)
    MeshComp->SetLeaderPoseComponent(CharacterMesh);

    // 캐릭터 메시에 부착
    MeshComp->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

    // 머티리얼 적용
    ApplyMaterials(MeshComp, Item->EquipmentMaterials);

    // 컴포넌트 등록
    AttachedSkeletalMeshes.Add(Slot, MeshComp);
}

USkeletalMeshComponent* UPGEquipmentVisualizerComponent::CreateSkeletalMeshComponent(UPGEquipmentItem* Item)
{
    if (!Item || !GetOwner())
    {
        return nullptr;
    }

    // 스켈레탈 메시 로드
    USkeletalMesh* SkeletalMesh = Item->EquipmentMesh.LoadSynchronous();
    if (!SkeletalMesh)
    {
        return nullptr;
    }

    // 컴포넌트 생성
    USkeletalMeshComponent* MeshComp = NewObject<USkeletalMeshComponent>(GetOwner(), USkeletalMeshComponent::StaticClass());
    MeshComp->SetSkeletalMesh(SkeletalMesh);
    MeshComp->SetVisibility(true);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->RegisterComponent();

    return MeshComp;
}

UStaticMeshComponent* UPGEquipmentVisualizerComponent::CreateStaticMeshComponent(UPGEquipmentItem* Item)
{
    if (!Item || !GetOwner())
    {
        return nullptr;
    }

    // 스태틱 메시 로드
    UStaticMesh* StaticMesh = Item->StaticEquipmentMesh.LoadSynchronous();
    if (!StaticMesh)
    {
        return nullptr;
    }

    // 컴포넌트 생성
    UStaticMeshComponent* MeshComp = NewObject<UStaticMeshComponent>(GetOwner(), UStaticMeshComponent::StaticClass());
    MeshComp->SetStaticMesh(StaticMesh);
    MeshComp->SetVisibility(true);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComp->RegisterComponent();

    return MeshComp;
}

void UPGEquipmentVisualizerComponent::ApplyMaterials(UMeshComponent* MeshComponent, const TArray<TSoftObjectPtr<UMaterialInterface>>& Materials)
{
    if (!MeshComponent || Materials.Num() == 0)
    {
        return;
    }

    // 각 머티리얼 슬롯에 머티리얼 적용
    for (int32 i = 0; i < Materials.Num(); ++i)
    {
        if (!Materials[i].IsNull())
        {
            UMaterialInterface* Material = Materials[i].LoadSynchronous();
            if (Material)
            {
                MeshComponent->SetMaterial(i, Material);
            }
        }
    }
}
