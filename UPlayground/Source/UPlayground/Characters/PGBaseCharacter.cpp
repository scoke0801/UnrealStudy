#include "PGBaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "../Combat/PGCombatComponent.h"
#include "../Abilities/PGAbilityComponent.h"

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
