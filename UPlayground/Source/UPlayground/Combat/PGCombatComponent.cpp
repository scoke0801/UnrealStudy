#include "PGCombatComponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

UPGCombatComponent::UPGCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
    
    // 기본 스탯 설정
    AttackPower = 10.0f;
    DefensePower = 5.0f;
    CriticalChance = 0.05f;
    
    // 기본 공격 설정
    AttackRange = 200.0f;
    AttackRadius = 50.0f;
    PrimaryAttackDamage = 10.0f;
    SecondaryAttackDamage = 15.0f;
    AttackCooldown = 1.0f;
    AttackCooldownRemaining = 0.0f;
}

void UPGCombatComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UPGCombatComponent, TargetActor);
    DOREPLIFETIME(UPGCombatComponent, AttackPower);
    DOREPLIFETIME(UPGCombatComponent, DefensePower);
    DOREPLIFETIME(UPGCombatComponent, CriticalChance);
}

void UPGCombatComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UPGCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 쿨다운 업데이트
    if (AttackCooldownRemaining > 0.0f)
    {
        AttackCooldownRemaining = FMath::Max(0.0f, AttackCooldownRemaining - DeltaTime);
    }
    
    // 타겟이 유효한지 주기적으로 확인
    if (TargetActor)
    {
        if (!IsValid(TargetActor))
        {
            ClearTarget();
        }
        else
        {
            // 타겟이 범위를 벗어났는지 확인 (필요한 경우)
            float DistanceToTarget = FVector::Distance(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
            if (DistanceToTarget > AttackRange * 2.0f) // 공격 범위의 두 배
            {
                // 필요한 경우 타겟 해제 또는 타겟 풀기 로직
            }
        }
    }
}

void UPGCombatComponent::OnRep_Target()
{
    // 타겟이 변경되었을 때 클라이언트에서 호출
    OnTargetChanged.Broadcast(TargetActor);
}

bool UPGCombatComponent::PrimaryAttack()
{
    return PerformAttack(PrimaryAttackDamage, PrimaryAttackMontage);
}

bool UPGCombatComponent::SecondaryAttack()
{
    return PerformAttack(SecondaryAttackDamage, SecondaryAttackMontage);
}

void UPGCombatComponent::SetTarget(AActor* NewTarget)
{
    if (GetOwnerRole() == ROLE_Authority)
    {
        TargetActor = NewTarget;
        OnTargetChanged.Broadcast(TargetActor);
    }
}

AActor* UPGCombatComponent::GetTarget() const
{
    return TargetActor;
}

void UPGCombatComponent::ClearTarget()
{
    SetTarget(nullptr);
}

void UPGCombatComponent::SelectTarget()
{
    if (GetOwnerRole() != ROLE_Authority)
    {
        // 클라이언트에서는 서버에 요청
        // 여기서는 로컬 처리를 하지만, 실제 구현에서는 서버 RPC를 호출해야 함
        return;
    }
    
    // 가장 가까운 적을 타겟으로 설정
    TArray<AActor*> PotentialTargets = FindPotentialTargets(AttackRange * 2.0f, 10);
    
    if (PotentialTargets.Num() > 0)
    {
        // 가장 가까운 적을 찾음
        AActor* ClosestTarget = nullptr;
        float ClosestDistance = FLT_MAX;
        FVector OwnerLocation = GetOwner()->GetActorLocation();
        
        for (AActor* Actor : PotentialTargets)
        {
            float Distance = FVector::Distance(OwnerLocation, Actor->GetActorLocation());
            if (Distance < ClosestDistance)
            {
                ClosestDistance = Distance;
                ClosestTarget = Actor;
            }
        }
        
        if (ClosestTarget)
        {
            SetTarget(ClosestTarget);
        }
    }
}

TArray<AActor*> UPGCombatComponent::FindPotentialTargets(float Range, int32 MaxTargets)
{
    TArray<AActor*> Result;
    
    UWorld* World = GetWorld();
    if (!World || !GetOwner())
    {
        return Result;
    }
    
    // 무시할 액터 (자기 자신)
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(GetOwner());
    
    // 구체 트레이스를 사용하여 잠재적 타겟 찾기
    TArray<FHitResult> HitResults;
    FVector Start = GetOwner()->GetActorLocation();
    ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Pawn);
    
    bool bHit = UKismetSystemLibrary::SphereTraceMulti(
        World,
        Start,
        Start,
        Range,
        TraceType,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        HitResults,
        true
    );
    
    if (bHit)
    {
        // 히트 결과에서 액터 추출
        for (const FHitResult& Hit : HitResults)
        {
            if (Hit.GetActor() && !Result.Contains(Hit.GetActor()))
            {
                // 여기서는 모든 액터를 타겟으로 간주하지만,
                // 실제 구현에서는 팀, 진영 등을 확인하여 적만 필터링해야 함
                Result.Add(Hit.GetActor());
                
                if (Result.Num() >= MaxTargets)
                {
                    break;
                }
            }
        }
    }
    
    return Result;
}

float UPGCombatComponent::GetAttackPower() const
{
    return AttackPower;
}

float UPGCombatComponent::GetDefensePower() const
{
    return DefensePower;
}

float UPGCombatComponent::GetCriticalChance() const
{
    return CriticalChance;
}

void UPGCombatComponent::SetAttackPower(float NewValue)
{
    if (GetOwnerRole() == ROLE_Authority)
    {
        AttackPower = FMath::Max(0.0f, NewValue);
    }
}

void UPGCombatComponent::SetDefensePower(float NewValue)
{
    if (GetOwnerRole() == ROLE_Authority)
    {
        DefensePower = FMath::Max(0.0f, NewValue);
    }
}

void UPGCombatComponent::SetCriticalChance(float NewValue)
{
    if (GetOwnerRole() == ROLE_Authority)
    {
        CriticalChance = FMath::Clamp(NewValue, 0.0f, 1.0f);
    }
}

float UPGCombatComponent::ApplyDamage(AActor* Target, float DamageAmount, TSubclassOf<UDamageType> DamageType)
{
    if (!Target || GetOwnerRole() != ROLE_Authority)
    {
        return 0.0f;
    }
    
    // 기본 데미지를 공격력으로 증가
    float FinalDamage = DamageAmount + (DamageAmount * (AttackPower / 100.0f));
    
    // 치명타 계산
    bool bIsCritical = FMath::FRand() <= CriticalChance;
    if (bIsCritical)
    {
        FinalDamage *= 2.0f; // 치명타는 데미지 2배
    }
    
    // 데미지 적용
    if (!DamageType)
    {
        // 기본 데미지 타입 사용
        DamageType = UDamageType::StaticClass();
    }
    
    return UGameplayStatics::ApplyDamage(Target, FinalDamage, GetOwner()->GetInstigatorController(), GetOwner(), DamageType);
}

bool UPGCombatComponent::CanAttack() const
{
    // 공격 쿨다운 확인
    if (AttackCooldownRemaining > 0.0f)
    {
        return false;
    }
    
    // 캐릭터 상태 확인 (필요한 경우)
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter)
    {
        // 캐릭터가 특정 상태에 있을 때 공격 불가
        // 예: 스턴, 사망 등
    }
    
    return true;
}

void UPGCombatComponent::StartAttackCooldown()
{
    AttackCooldownRemaining = AttackCooldown;
    
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            AttackCooldownTimerHandle,
            this,
            &UPGCombatComponent::OnAttackCooldownEnd,
            AttackCooldown,
            false
        );
    }
}

void UPGCombatComponent::OnAttackCooldownEnd()
{
    AttackCooldownRemaining = 0.0f;
}

bool UPGCombatComponent::PerformAttack(float Damage, UAnimMontage* AttackMontage)
{
    if (!CanAttack() || !AttackMontage)
    {
        return false;
    }
    
    // 몽타주 재생
    PlayAttackMontage(AttackMontage);
    
    // 쿨다운 시작
    StartAttackCooldown();
    
    // 타겟이 있으면 데미지 처리
    if (TargetActor && GetOwnerRole() == ROLE_Authority)
    {
        // 몽타주에서 처리할 수도 있지만, 여기서는 즉시 데미지 적용
        float Distance = FVector::Distance(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
        if (Distance <= AttackRange)
        {
            ApplyDamage(TargetActor, Damage, nullptr);
        }
    }
    
    return true;
}

void UPGCombatComponent::PlayAttackMontage(UAnimMontage* Montage, FName Section)
{
    UAnimInstance* AnimInstance = GetOwnerAnimInstance();
    if (AnimInstance && Montage)
    {
        float PlayRate = 1.0f;
        if (Section != NAME_None)
        {
            AnimInstance->Montage_Play(Montage, PlayRate);
            AnimInstance->Montage_JumpToSection(Section, Montage);
        }
        else
        {
            AnimInstance->Montage_Play(Montage, PlayRate);
        }
    }
}

void UPGCombatComponent::OnAttackHit(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    // 몽타주에서 히트 노티파이를 받았을 때 처리
    // "AttackHit" 등의 노티파이 이름으로 호출
    
    if (NotifyName == FName("AttackHit") && GetOwnerRole() == ROLE_Authority)
    {
        // 타겟이 없으면 범위 내 모든 적에게 데미지
        if (!TargetActor)
        {
            // 공격 범위 내 모든 액터 찾기
            TArray<AActor*> TargetsInRange = FindPotentialTargets(AttackRange, 5);
            
            // 각 액터에 데미지 적용
            for (AActor* Actor : TargetsInRange)
            {
                ApplyDamage(Actor, PrimaryAttackDamage, nullptr);
            }
        }
        // 타겟이 있으면 타겟에만 데미지
        else
        {
            float Distance = FVector::Distance(GetOwner()->GetActorLocation(), TargetActor->GetActorLocation());
            if (Distance <= AttackRange)
            {
                ApplyDamage(TargetActor, PrimaryAttackDamage, nullptr);
            }
        }
    }
}

UAnimInstance* UPGCombatComponent::GetOwnerAnimInstance() const
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
    if (OwnerCharacter && OwnerCharacter->GetMesh())
    {
        return OwnerCharacter->GetMesh()->GetAnimInstance();
    }
    
    return nullptr;
}
