#include "PGCharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"

UPGCharacterMovementComponent::UPGCharacterMovementComponent()
{
    // 기본값 설정
    DodgeStrength = 2000.0f;
    DodgeDuration = 0.2f;
    DodgeCooldown = 1.0f;
    SprintSpeedMultiplier = 1.5f;
    DefaultWalkSpeed = 600.0f;
    
    // 이동 상태 초기화
    CurrentMovementState = EPGMovementState::Walking;
    bIsDodging = false;
    bDodgeOnCooldown = false;
}

void UPGCharacterMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UPGCharacterMovementComponent, CurrentMovementState);
}

EPGMovementState UPGCharacterMovementComponent::GetMovementState() const
{
    return CurrentMovementState;
}

void UPGCharacterMovementComponent::SetMovementState(EPGMovementState NewState)
{
    if (GetOwnerRole() == ROLE_Authority)
    {
        CurrentMovementState = NewState;
    }
}

bool UPGCharacterMovementComponent::CanDodge() const
{
    return !bIsDodging && !bDodgeOnCooldown && IsMovingOnGround();
}

void UPGCharacterMovementComponent::Dodge(FVector Direction)
{
    if (!CanDodge() || !CharacterOwner)
    {
        return;
    }
    
    // 회피 방향 정규화
    DodgeDirection = Direction.GetSafeNormal();
    
    // 현재 이동 모드 저장 및 상태 변경
    SetMovementState(EPGMovementState::Dodging);
    bIsDodging = true;
    
    // 회피 로직 시작
    if (GetOwnerRole() == ROLE_Authority)
    {
        // 회피 강도로 캐릭터 밀기
        FVector DodgeVelocity = DodgeDirection * DodgeStrength;
        Launch(DodgeVelocity);
        
        // 회피 타이머 설정
        GetWorld()->GetTimerManager().SetTimer(
            DodgeTimerHandle,
            [this]()
            {
                // 회피 종료
                bIsDodging = false;
                SetMovementState(EPGMovementState::Walking);
                
                // 쿨다운 시작
                bDodgeOnCooldown = true;
                GetWorld()->GetTimerManager().SetTimer(
                    DodgeCooldownTimerHandle,
                    [this]()
                    {
                        bDodgeOnCooldown = false;
                    },
                    DodgeCooldown,
                    false
                );
            },
            DodgeDuration,
            false
        );
    }
}

void UPGCharacterMovementComponent::StartSprinting()
{
    if (CurrentMovementState != EPGMovementState::Walking)
    {
        return;
    }
    
    // 달리기 상태로 변경
    SetMovementState(EPGMovementState::Sprinting);
    
    // 이동 속도 증가
    MaxWalkSpeed = DefaultWalkSpeed * SprintSpeedMultiplier;
}

void UPGCharacterMovementComponent::StopSprinting()
{
    if (CurrentMovementState != EPGMovementState::Sprinting)
    {
        return;
    }
    
    // 걷기 상태로 변경
    SetMovementState(EPGMovementState::Walking);
    
    // 이동 속도 복원
    MaxWalkSpeed = DefaultWalkSpeed;
}

void UPGCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
    // 기본 걷기 물리
    Super::PhysWalking(deltaTime, Iterations);
}

void UPGCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
    // 회피 중인 경우 커스텀 물리 적용
    if (bIsDodging)
    {
        PerformDodge(deltaTime);
    }
    else
    {
        Super::PhysCustom(deltaTime, Iterations);
    }
}

void UPGCharacterMovementComponent::PerformDodge(float DeltaTime)
{
    // 회피 중 물리 계산
    // 여기서는 기본 물리를 사용하지만, 커스텀 물리 로직을 추가할 수 있음
    
    // 중력 적용
    if (IsMovingOnGround())
    {
        // 지면에서는 표준 지상 이동 사용
        CalcVelocity(DeltaTime, GroundFriction, false, BrakingDecelerationWalking);
    }
    else
    {
        // 공중에서는 공중 이동 로직 사용
        CalcVelocity(DeltaTime, FallingLateralFriction, false, BrakingDecelerationFalling);
        Velocity.Z += GetGravityZ() * DeltaTime;
    }
    
    // 이동 적용
    FVector Delta = Velocity * DeltaTime;
    FHitResult Hit(1.f);
    SafeMoveUpdatedComponent(Delta, UpdatedComponent->GetComponentQuat(), true, Hit);
    
    // 충돌 처리
    if (Hit.IsValidBlockingHit())
    {
        HandleImpact(Hit, DeltaTime, Delta);
        SlideAlongSurface(Delta, 1.0f - Hit.Time, Hit.Normal, Hit, true);
    }
}
