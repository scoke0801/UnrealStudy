#include "PGAbility.h"
#include "PGAbilityComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"

UPGAbility::UPGAbility()
{
    // 기본값 설정
    AbilityID = NAME_None;
    AbilityName = FText::FromString("Default Ability");
    AbilityDescription = FText::FromString("Default ability description");
    
    // 기본 요구사항
    bRequiresTarget = false;
    Range = 0.0f;
    
    // 기본 쿨다운
    CooldownDuration = 1.0f;
    CooldownTimeRemaining = 0.0f;
    
    // 초기 상태
    CurrentState = EPGAbilityState::Inactive;
}

void UPGAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UPGAbility, CurrentState);
    DOREPLIFETIME(UPGAbility, CooldownTimeRemaining);
}

EPGAbilityState UPGAbility::GetAbilityState() const
{
    return CurrentState;
}

bool UPGAbility::IsActive() const
{
    return CurrentState == EPGAbilityState::Active || CurrentState == EPGAbilityState::Activating;
}

bool UPGAbility::IsOnCooldown() const
{
    return CurrentState == EPGAbilityState::Cooldown;
}

float UPGAbility::GetCooldownTimeRemaining() const
{
    return CooldownTimeRemaining;
}

float UPGAbility::GetCooldownPercent() const
{
    if (CooldownDuration <= 0.0f)
    {
        return 0.0f;
    }
    
    return FMath::Clamp(CooldownTimeRemaining / CooldownDuration, 0.0f, 1.0f);
}

bool UPGAbility::CanActivate() const
{
    if (!OwningActor || !AbilityComponent)
    {
        return false;
    }
    
    // 이미 활성화되어 있거나 쿨다운 중이면 활성화 불가
    if (IsActive() || IsOnCooldown())
    {
        return false;
    }
    
    // 타겟이 필요한데 타겟이 없으면 활성화 불가
    if (bRequiresTarget)
    {
        // 타겟 확인 로직 필요
        // 예: 전투 컴포넌트에서 현재 타겟 확인
    }
    
    return true;
}

bool UPGAbility::Activate()
{
    if (!CanActivate())
    {
        return false;
    }
    
    // 상태 변경
    CurrentState = EPGAbilityState::Activating;
    
    // 블루프린트 이벤트 호출
    OnActivate();
    K2_OnActivate();
    
    // 활성화 완료
    CurrentState = EPGAbilityState::Active;
    
    return true;
}

void UPGAbility::End(bool bWasCancelled)
{
    if (!IsActive())
    {
        return;
    }
    
    // 블루프린트 이벤트 호출
    OnEnd(bWasCancelled);
    K2_OnEnd(bWasCancelled);
    
    // 쿨다운 시작
    StartCooldown();
    
    // 이벤트 발생
    if (AbilityComponent)
    {
        AbilityComponent->OnAbilityEnded.Broadcast(this, bWasCancelled);
    }
}

void UPGAbility::OnRep_AbilityState()
{
    // 클라이언트에서 상태 변경 처리
    switch (CurrentState)
    {
    case EPGAbilityState::Activating:
    case EPGAbilityState::Active:
        // 이미 활성화된 상태
        break;
        
    case EPGAbilityState::Cooldown:
        // 쿨다운 시각 효과 등
        break;
        
    case EPGAbilityState::Inactive:
        // 비활성화 상태
        break;
    }
}

void UPGAbility::K2_OnActivate_Implementation()
{
    // 기본 네이티브 구현 (빈 함수)
}

void UPGAbility::K2_OnEnd_Implementation(bool bWasCancelled)
{
    // 기본 네이티브 구현 (빈 함수)
}

void UPGAbility::StartCooldown()
{
    if (CooldownDuration <= 0.0f)
    {
        // 쿨다운이 없으면 바로 비활성화
        CurrentState = EPGAbilityState::Inactive;
        return;
    }
    
    // 상태 업데이트
    CurrentState = EPGAbilityState::Cooldown;
    CooldownTimeRemaining = CooldownDuration;
    
    // 쿨다운 타이머 시작
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            CooldownTimerHandle,
            this,
            &UPGAbility::OnCooldownEnd,
            CooldownDuration,
            false
        );
        
        // 쿨다운 시작 이벤트 발생
        if (AbilityComponent)
        {
            AbilityComponent->OnCooldownStarted.Broadcast(AbilityID, CooldownDuration);
        }
    }
}

void UPGAbility::ClearCooldown()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(CooldownTimerHandle);
    }
    
    CooldownTimeRemaining = 0.0f;
    
    if (CurrentState == EPGAbilityState::Cooldown)
    {
        CurrentState = EPGAbilityState::Inactive;
        
        // 쿨다운 종료 이벤트 발생
        if (AbilityComponent)
        {
            AbilityComponent->OnCooldownEnded.Broadcast(AbilityID);
        }
    }
}

void UPGAbility::OnCooldownEnd()
{
    CooldownTimeRemaining = 0.0f;
    CurrentState = EPGAbilityState::Inactive;
    
    // 쿨다운 종료 이벤트 발생
    if (AbilityComponent)
    {
        AbilityComponent->OnCooldownEnded.Broadcast(AbilityID);
    }
}
