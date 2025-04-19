// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGBaseCharacter.h"
#include "../Items/PGInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

APGBaseCharacter::APGBaseCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // 기본 속성 초기화
    MaxHealth = 100.0f;
    CurrentHealth = MaxHealth;
    MaxMana = 100.0f;
    CurrentMana = MaxMana;
    Level = 1;
    MovementSpeed = 600.0f;

    // 인벤토리 컴포넌트 생성
    InventoryComponent = CreateDefaultSubobject<UPGInventoryComponent>(TEXT("InventoryComponent"));
}

void APGBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    // 이동 속도 설정
    GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
}

void APGBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APGBaseCharacter::ApplyDamageToCharacter(float DamageAmount, AActor* DamageCauser)
{
    if (!IsAlive()) return;

    CurrentHealth = FMath::Max(CurrentHealth - DamageAmount, 0.0f);
    
    // 데미지 이벤트 발생
    OnCharacterDamaged.Broadcast(this, DamageAmount);
    
    if (CurrentHealth <= 0.0f)
    {
        Die();
    }
}

void APGBaseCharacter::Die()
{
    // 사망 이벤트 발생
    OnCharacterDied.Broadcast(this);
    
    // 구체적인 사망 로직은 자식 클래스에서 구현
}

void APGBaseCharacter::Heal(float HealAmount)
{
    if (!IsAlive()) return;
    
    CurrentHealth = FMath::Min(CurrentHealth + HealAmount, MaxHealth);
}

bool APGBaseCharacter::IsAlive() const
{
    return CurrentHealth > 0.0f;
}

void APGBaseCharacter::LevelUp()
{
    Level++;
    
    // 레벨업 이벤트 발생
    OnCharacterLevelUp.Broadcast(this);
    
    // 스탯 증가 등의 로직은 자식 클래스에서 구현
}

void APGBaseCharacter::Interact(APGBaseCharacter* Interactor)
{
    // 기본 상호작용 로직 - 자식 클래스에서 오버라이드
}

bool APGBaseCharacter::CanBeInteractedWith() const
{
    return IsAlive(); // 기본적으로 살아있는 캐릭터만 상호작용 가능
}