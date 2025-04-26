#include "PGBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "../Combat/PGCombatComponent.h"
#include "../Abilities/PGAbilityComponent.h"
#include "../Items/PGInventoryComponent.h"
#include "../Equipment/PGEquipmentComponent.h"
#include "../Equipment/PGEquipmentVisualizerComponent.h"
#include "../Equipment/PGEquipmentItem.h"

APGBaseCharacter::APGBaseCharacter()
{
    // 기본값 설정
    PrimaryActorTick.bCanEverTick = true;
    
    // 초기 상태 설정
    CurrentState = EPGCharacterState::Idle;
    
    // 기본 체력 설정
    MaxHealth = 100.0f;
    Health = MaxHealth;
    
    // 컴포넌트 생성
    CombatComponent = CreateDefaultSubobject<UPGCombatComponent>(TEXT("CombatComponent"));
    AbilityComponent = CreateDefaultSubobject<UPGAbilityComponent>(TEXT("AbilityComponent"));
    InventoryComponent = CreateDefaultSubobject<UPGInventoryComponent>(TEXT("InventoryComponent"));
    EquipmentComponent = CreateDefaultSubobject<UPGEquipmentComponent>(TEXT("EquipmentComponent"));
    EquipmentVisualizerComponent = CreateDefaultSubobject<UPGEquipmentVisualizerComponent>(TEXT("EquipmentVisualizerComponent"));
}

void APGBaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    // 복제할 속성 등록
    DOREPLIFETIME(APGBaseCharacter, CurrentState);
    DOREPLIFETIME(APGBaseCharacter, Health);
}

EPGCharacterState APGBaseCharacter::GetCharacterState() const
{
    return CurrentState;
}

void APGBaseCharacter::SetCharacterState(EPGCharacterState NewState)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        CurrentState = NewState;
    }
}

float APGBaseCharacter::GetHealth() const
{
    return Health;
}

float APGBaseCharacter::GetMaxHealth() const
{
    return MaxHealth;
}

void APGBaseCharacter::SetHealth(float NewHealth)
{
    if (GetLocalRole() == ROLE_Authority)
    {
        Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
        
        // 체력이 0이 되면 사망 상태로 전환
        if (Health <= 0.0f && CurrentState != EPGCharacterState::Dead)
        {
            SetCharacterState(EPGCharacterState::Dead);
        }
    }
}

float APGBaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, 
                                   AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    if (ActualDamage > 0.0f && GetLocalRole() == ROLE_Authority)
    {
        SetHealth(GetHealth() - ActualDamage);
    }
    
    return ActualDamage;
}

void APGBaseCharacter::Interact(APGBaseCharacter* Interactor)
{
    // 기본 상호작용 구현, 자식 클래스에서 재정의 가능
}

bool APGBaseCharacter::CanBeInteractedWith() const
{
    // 기본적으로 상호작용 가능
    return true;
}

void APGBaseCharacter::OnRep_CharacterState()
{
    // 캐릭터 상태가 변경되었을 때 클라이언트에서 실행되는 로직
    // 예: 애니메이션 재생, 파티클 효과 등
    
    if (CurrentState == EPGCharacterState::Dead)
    {
        // 사망 처리
        // 예: 레그돌 활성화, 충돌 비활성화 등
    }
}

bool APGBaseCharacter::EquipItemFromInventory(UPGEquipmentItem* Item)
{
    if (!Item || !InventoryComponent || !EquipmentComponent)
    {
        return false;
    }

    // 아이템이 장비 타입인지 확인
    if (Item->GetItemType() != EPGItemType::Equipment)
    {
        return false;
    }

    // 인벤토리에 해당 아이템이 있는지 확인
    TArray<int32> SlotIndices;
    if (!InventoryComponent->FindItemsByID(Item->GetItemID(), SlotIndices) || SlotIndices.Num() == 0)
    {
        return false;
    }

    // 장비 장착
    if (EquipmentComponent->EquipItem(Item))
    {
        // 인벤토리에서 아이템 제거
        InventoryComponent->RemoveItem(SlotIndices[0], 1);
        return true;
    }

    return false;
}

bool APGBaseCharacter::UnequipItemToInventory(EPGEquipmentSlot Slot)
{
    if (!InventoryComponent || !EquipmentComponent)
    {
        return false;
    }

    // 해당 슬롯의 장비 가져오기
    UPGEquipmentItem* Item = EquipmentComponent->GetEquippedItem(Slot);
    if (!Item)
    {
        return false;
    }

    // 인벤토리에 공간이 있는지 확인
    int32 OutSlotIndex = -1;
    if (!InventoryComponent->AddItem(Item, OutSlotIndex, 1))
    {
        return false;
    }

    // 장비 해제
    if (EquipmentComponent->UnequipItem(Slot))
    {
        return true;
    }

    // 장비 해제에 실패했다면 인벤토리에서도 제거
    InventoryComponent->RemoveItem(OutSlotIndex, 1);
    return false;
}
