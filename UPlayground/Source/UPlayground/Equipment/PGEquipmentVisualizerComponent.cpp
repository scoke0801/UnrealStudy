// Fill out your copyright notice in the Description page of Project Settings.

#include "PGEquipmentVisualizerComponent.h"
#include "GameFramework/Character.h"
#include "PGEquipmentComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

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

    // 아이템 레퍼런스 저장
    EquippedItemRefs.Add(Slot, Item);

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
    
    // 아이템 레퍼런스 제거
    if (EquippedItemRefs.Contains(Slot))
    {
        EquippedItemRefs.Remove(Slot);
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

bool UPGEquipmentVisualizerComponent::ApplyDyeToEquipment(EPGEquipmentSlot Slot, EPGDyeChannel Channel, const FPGDyeInfo& DyeInfo)
{
    // 해당 슬롯에 장비가 있는지 확인
    if (!EquippedItemRefs.Contains(Slot))
    {
        return false;
    }
    
    // 장비 아이템 가져오기
    UPGEquipmentItem* EquipmentItem = EquippedItemRefs[Slot];
    if (!EquipmentItem || !EquipmentItem->IsDyeable())
    {
        return false;
    }
    
    // 염색 적용
    bool bSuccess = EquipmentItem->ApplyDye(Channel, DyeInfo);
    if (bSuccess)
    {
        // 시각적으로 염색 업데이트
        UpdateDyeVisual(Slot, Channel);
        
        // 염색 변경 이벤트 발생
        OnEquipmentDyeChanged.Broadcast(Slot, Channel, DyeInfo);
    }
    
    return bSuccess;
}

void UPGEquipmentVisualizerComponent::UpdateDyeVisual(EPGEquipmentSlot Slot, EPGDyeChannel Channel)
{
    // 해당 슬롯에 장비가 있는지 확인
    if (!EquippedItemRefs.Contains(Slot))
    {
        return;
    }
    
    UPGEquipmentItem* Item = EquippedItemRefs[Slot];
    if (!Item)
    {
        return;
    }
    
    // 메시 컴포넌트 찾기
    UMeshComponent* MeshComponent = nullptr;
    
    if (AttachedSkeletalMeshes.Contains(Slot))
    {
        MeshComponent = AttachedSkeletalMeshes[Slot];
    }
    else if (AttachedStaticMeshes.Contains(Slot))
    {
        MeshComponent = AttachedStaticMeshes[Slot];
    }
    
    if (!MeshComponent)
    {
        return;
    }
    
    // 특정 채널 염색 적용
    ApplyDyeChannel(MeshComponent, Item, Channel);
}

void UPGEquipmentVisualizerComponent::ResetAllDyes()
{
    // 모든 장비의 염색 초기화
    for (auto& ItemPair : EquippedItemRefs)
    {
        UPGEquipmentItem* Item = ItemPair.Value;
        if (Item && Item->IsDyeable())
        {
            // 모든 염색 제거
            Item->ClearAllDyes();
            
            // 메시 컴포넌트 찾기
            UMeshComponent* MeshComponent = nullptr;
            
            if (AttachedSkeletalMeshes.Contains(ItemPair.Key))
            {
                MeshComponent = AttachedSkeletalMeshes[ItemPair.Key];
            }
            else if (AttachedStaticMeshes.Contains(ItemPair.Key))
            {
                MeshComponent = AttachedStaticMeshes[ItemPair.Key];
            }
            
            if (MeshComponent)
            {
                // 염색 재적용
                ApplyDyesToMeshComponent(MeshComponent, Item);
            }
        }
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
    
    // 염색 적용
    if (Item->IsDyeable())
    {
        ApplyDyesToMeshComponent(MeshComp, Item);
    }

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
    
    // 염색 적용
    if (Item->IsDyeable())
    {
        ApplyDyesToMeshComponent(MeshComp, Item);
    }

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
                // 동적 머티리얼 인스턴스 생성 (염색을 위해)
                UMaterialInstanceDynamic* DynamicMaterial = UMaterialInstanceDynamic::Create(Material, MeshComponent);
                if (DynamicMaterial)
                {
                    MeshComponent->SetMaterial(i, DynamicMaterial);
                }
                else
                {
                    MeshComponent->SetMaterial(i, Material);
                }
            }
        }
    }
}

void UPGEquipmentVisualizerComponent::ApplyDyesToMeshComponent(UMeshComponent* MeshComponent, UPGEquipmentItem* Item)
{
    if (!MeshComponent || !Item || !Item->IsDyeable())
    {
        return;
    }
    
    // 각 염색 채널에 대해 염색 적용
    for (const EPGDyeChannel& Channel : Item->AvailableDyeChannels)
    {
        ApplyDyeChannel(MeshComponent, Item, Channel);
    }
}

void UPGEquipmentVisualizerComponent::ApplyDyeChannel(UMeshComponent* MeshComponent, UPGEquipmentItem* Item, EPGDyeChannel Channel)
{
    if (!MeshComponent || !Item)
    {
        return;
    }
    
    // 해당 채널의 염색 정보 가져오기
    FPGDyeInfo DyeInfo = Item->GetDyeInfo(Channel);
    
    // 파라미터 이름 가져오기
    FName ColorParamName = NAME_None;
    FName MetallicParamName = NAME_None;
    FName RoughnessParamName = NAME_None;
    
    if (Item->DyeParameterNames.Contains(Channel))
    {
        ColorParamName = Item->DyeParameterNames[Channel];
    }
    
    if (Item->MetallicParameterNames.Contains(Channel))
    {
        MetallicParamName = Item->MetallicParameterNames[Channel];
    }
    
    if (Item->RoughnessParameterNames.Contains(Channel))
    {
        RoughnessParamName = Item->RoughnessParameterNames[Channel];
    }
    
    // 모든 머티리얼에 파라미터 설정
    int32 MaterialCount = MeshComponent->GetNumMaterials();
    for (int32 i = 0; i < MaterialCount; ++i)
    {
        UMaterialInstanceDynamic* DynamicMaterial = Cast<UMaterialInstanceDynamic>(MeshComponent->GetMaterial(i));
        
        // 동적 머티리얼이 아니라면 생성
        if (!DynamicMaterial)
        {
            UMaterialInterface* BaseMaterial = MeshComponent->GetMaterial(i);
            if (BaseMaterial)
            {
                DynamicMaterial = UMaterialInstanceDynamic::Create(BaseMaterial, MeshComponent);
                if (DynamicMaterial)
                {
                    MeshComponent->SetMaterial(i, DynamicMaterial);
                }
                else
                {
                    continue;
                }
            }
        }
        
        // 파라미터 설정
        if (DynamicMaterial)
        {
            // 색상 파라미터
            if (!ColorParamName.IsNone())
            {
                DynamicMaterial->SetVectorParameterValue(ColorParamName, DyeInfo.Color);
            }
            
            // 메탈릭 파라미터
            if (!MetallicParamName.IsNone())
            {
                DynamicMaterial->SetScalarParameterValue(MetallicParamName, DyeInfo.Metallic);
            }
            
            // 광택 파라미터
            if (!RoughnessParamName.IsNone())
            {
                DynamicMaterial->SetScalarParameterValue(RoughnessParamName, DyeInfo.Roughness);
            }
        }
    }
}
