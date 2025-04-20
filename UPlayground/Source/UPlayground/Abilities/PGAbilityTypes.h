#pragma once

#include "CoreMinimal.h"
#include "PGAbilityTypes.generated.h"

// 리소스 타입
UENUM(BlueprintType)
enum class EPGResourceType : uint8
{
    Mana,
    Stamina,
    Rage,
    Energy,
    Health
};

// 능력 대상 타입
UENUM(BlueprintType)
enum class EPGAbilityTargetType : uint8
{
    Self,
    SingleTarget,
    AOE,
    Cone,
    Line
};

// 능력 효과 타입
UENUM(BlueprintType)
enum class EPGAbilityEffectType : uint8
{
    Damage,
    Heal,
    Buff,
    Debuff,
    ControlEffect,
    Movement,
    SpawnActor
};

// 리소스 비용 구조체
USTRUCT(BlueprintType)
struct FPGResourceCost
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EPGResourceType ResourceType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Cost;

    // 기본 생성자
    FPGResourceCost()
        : ResourceType(EPGResourceType::Mana)
        , Cost(0.0f)
    {
    }

    // 파라미터를 받는 생성자
    FPGResourceCost(EPGResourceType InResourceType, float InCost)
        : ResourceType(InResourceType)
        , Cost(InCost)
    {
    }
};

// 이벤트 델리게이트 정의
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityActivated, class UPGAbility*, Ability);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityEnded, class UPGAbility*, Ability, bool, bWasCancelled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooldownStarted, FName, AbilityID, float, CooldownDuration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCooldownEnded, FName, AbilityID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResourceChanged, EPGResourceType, ResourceType, float, OldValue, float, NewValue);
