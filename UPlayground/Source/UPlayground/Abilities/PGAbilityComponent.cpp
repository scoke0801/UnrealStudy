#include "PGAbilityComponent.h"
#include "PGAbility.h"
#include "Net/UnrealNetwork.h"

UPGAbilityComponent::UPGAbilityComponent()
{
    // 컴포넌트 설정
    PrimaryComponentTick.bCanEverTick = true;
    SetIsReplicatedByDefault(true);
    
    // 기본 리소스 설정
    MaxResources.Add(EPGResourceType::Mana, 100.0f);
    MaxResources.Add(EPGResourceType::Stamina, 100.0f);
    MaxResources.Add(EPGResourceType::Rage, 100.0f);
    MaxResources.Add(EPGResourceType::Energy, 100.0f);
    
    // 기본 리소스 재생성 속도
    ResourceRegenRates.Add(EPGResourceType::Mana, 5.0f);
    ResourceRegenRates.Add(EPGResourceType::Stamina, 10.0f);
    ResourceRegenRates.Add(EPGResourceType::Rage, -10.0f); // 전투 중이 아닐 때는 감소
    ResourceRegenRates.Add(EPGResourceType::Energy, 8.0f);
}

void UPGAbilityComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UPGAbilityComponent, CurrentResources);
}

void UPGAbilityComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 리소스 초기화
    for (auto& Pair : MaxResources)
    {
        CurrentResources.Add(Pair.Key, Pair.Value);
    }
}

void UPGAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 서버에서만 리소스 재생성
    if (GetOwnerRole() == ROLE_Authority)
    {
        RegenerateResources(DeltaTime);
    }
}

bool UPGAbilityComponent::GiveAbility(TSubclassOf<UPGAbility> AbilityClass)
{
    if (!AbilityClass || GetOwnerRole() != ROLE_Authority)
    {
        return false;
    }
    
    // 능력 인스턴스 생성
    UPGAbility* NewAbility = CreateAbilityInstance(AbilityClass);
    if (!NewAbility)
    {
        return false;
    }
    
    // 이미 존재하는 능력인지 확인
    if (Abilities.Contains(NewAbility->AbilityID))
    {
        // 이미 가지고 있는 능력이면 교체
        Abilities[NewAbility->AbilityID]->MarkAsGarbage();
        Abilities[NewAbility->AbilityID] = NewAbility;
    }
    else
    {
        // 새 능력 추가
        Abilities.Add(NewAbility->AbilityID, NewAbility);
    }
    
    return true;
}

bool UPGAbilityComponent::RemoveAbility(FName AbilityID)
{
    if (!Abilities.Contains(AbilityID) || GetOwnerRole() != ROLE_Authority)
    {
        return false;
    }
    
    // 능력이 활성화 상태면 종료
    UPGAbility* Ability = Abilities[AbilityID];
    if (Ability && Ability->IsActive())
    {
        Ability->End(true);
    }
    
    // 능력 제거
    Abilities.Remove(AbilityID);
    
    return true;
}

UPGAbility* UPGAbilityComponent::GetAbility(FName AbilityID) const
{
    if (Abilities.Contains(AbilityID))
    {
        return Abilities[AbilityID];
    }
    
    return nullptr;
}

bool UPGAbilityComponent::ActivateAbility(FName AbilityID)
{
    if (!Abilities.Contains(AbilityID))
    {
        return false;
    }
    
    UPGAbility* Ability = Abilities[AbilityID];
    
    // 능력 활성화 가능 여부 확인
    if (!Ability->CanActivate())
    {
        return false;
    }
    
    // 리소스 비용 지불
    for (const auto& Cost : Ability->ResourceCosts)
    {
        if (!ConsumeResource(Cost.Key, Cost.Value))
        {
            return false;
        }
    }
    
    // 능력 활성화
    bool bActivated = Ability->Activate();
    
    if (bActivated)
    {
        OnAbilityActivated.Broadcast(Ability);
    }
    
    return bActivated;
}

bool UPGAbilityComponent::CanActivateAbility(FName AbilityID) const
{
    if (!Abilities.Contains(AbilityID))
    {
        return false;
    }
    
    UPGAbility* Ability = Abilities[AbilityID];
    
    // 능력 활성화 가능 여부 확인
    if (!Ability->CanActivate())
    {
        return false;
    }
    
    // 리소스 비용 지불 가능 여부 확인
    for (const auto& Cost : Ability->ResourceCosts)
    {
        if (GetResource(Cost.Key) < Cost.Value)
        {
            return false;
        }
    }
    
    return true;
}

float UPGAbilityComponent::GetResource(EPGResourceType ResourceType) const
{
    if (CurrentResources.Contains(ResourceType))
    {
        return CurrentResources[ResourceType];
    }
    
    return 0.0f;
}

float UPGAbilityComponent::GetMaxResource(EPGResourceType ResourceType) const
{
    if (MaxResources.Contains(ResourceType))
    {
        return MaxResources[ResourceType];
    }
    
    return 0.0f;
}

bool UPGAbilityComponent::ConsumeResource(EPGResourceType ResourceType, float Amount)
{
    if (GetOwnerRole() != ROLE_Authority)
    {
        return false;
    }
    
    if (!CurrentResources.Contains(ResourceType) || CurrentResources[ResourceType] < Amount)
    {
        return false;
    }
    
    float OldValue = CurrentResources[ResourceType];
    CurrentResources[ResourceType] -= Amount;
    
    OnResourceChanged.Broadcast(ResourceType, OldValue, CurrentResources[ResourceType]);
    
    return true;
}

void UPGAbilityComponent::SetResource(EPGResourceType ResourceType, float NewValue)
{
    if (GetOwnerRole() != ROLE_Authority)
    {
        return;
    }
    
    if (!MaxResources.Contains(ResourceType))
    {
        MaxResources.Add(ResourceType, NewValue);
    }
    
    float MaxValue = MaxResources[ResourceType];
    float ClampedValue = FMath::Clamp(NewValue, 0.0f, MaxValue);
    
    float OldValue = 0.0f;
    if (CurrentResources.Contains(ResourceType))
    {
        OldValue = CurrentResources[ResourceType];
        CurrentResources[ResourceType] = ClampedValue;
    }
    else
    {
        CurrentResources.Add(ResourceType, ClampedValue);
    }
    
    OnResourceChanged.Broadcast(ResourceType, OldValue, ClampedValue);
}

void UPGAbilityComponent::SetMaxResource(EPGResourceType ResourceType, float NewValue)
{
    if (GetOwnerRole() != ROLE_Authority)
    {
        return;
    }
    
    float OldMaxValue = 0.0f;
    if (MaxResources.Contains(ResourceType))
    {
        OldMaxValue = MaxResources[ResourceType];
        MaxResources[ResourceType] = FMath::Max(0.0f, NewValue);
    }
    else
    {
        MaxResources.Add(ResourceType, FMath::Max(0.0f, NewValue));
    }
    
    // 현재 리소스 값이 새 최대값보다 크면 조정
    if (CurrentResources.Contains(ResourceType) && CurrentResources[ResourceType] > MaxResources[ResourceType])
    {
        float OldValue = CurrentResources[ResourceType];
        CurrentResources[ResourceType] = MaxResources[ResourceType];
        
        OnResourceChanged.Broadcast(ResourceType, OldValue, CurrentResources[ResourceType]);
    }
}

void UPGAbilityComponent::OnRep_Resources()
{
    // 리소스 값이 복제되었을 때 호출
    // 클라이언트에서 이벤트 발생 처리
    for (const auto& Pair : CurrentResources)
    {
        // 클라이언트에서는 이전 값을 알 수 없으므로 현재 값을 두 번 전달
        OnResourceChanged.Broadcast(Pair.Key, Pair.Value, Pair.Value);
    }
}

void UPGAbilityComponent::RegenerateResources(float DeltaTime)
{
    // 각 리소스 타입에 대한 재생성 처리
    for (const auto& Pair : ResourceRegenRates)
    {
        EPGResourceType ResourceType = Pair.Key;
        float RegenRate = Pair.Value;
        
        if (CurrentResources.Contains(ResourceType) && MaxResources.Contains(ResourceType))
        {
            float CurrentValue = CurrentResources[ResourceType];
            float MaxValue = MaxResources[ResourceType];
            
            // 리소스가 최대가 아니고 재생성 속도가 양수이거나,
            // 리소스가 0이 아니고 재생성 속도가 음수인 경우에만 재생성
            if ((CurrentValue < MaxValue && RegenRate > 0.0f) || 
                (CurrentValue > 0.0f && RegenRate < 0.0f))
            {
                float NewValue = FMath::Clamp(CurrentValue + (RegenRate * DeltaTime), 0.0f, MaxValue);
                
                if (NewValue != CurrentValue)
                {
                    float OldValue = CurrentValue;
                    CurrentResources[ResourceType] = NewValue;
                    
                    OnResourceChanged.Broadcast(ResourceType, OldValue, NewValue);
                }
            }
        }
    }
}

UPGAbility* UPGAbilityComponent::CreateAbilityInstance(TSubclassOf<UPGAbility> AbilityClass)
{
    UPGAbility* Ability = NewObject<UPGAbility>(GetOwner(), AbilityClass);
    if (Ability)
    {
        Ability->AbilityComponent = this;
        Ability->OwningActor = GetOwner();
    }
    
    return Ability;
}
