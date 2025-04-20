#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "PGDamageType.generated.h"

// 데미지 타입 enum
UENUM(BlueprintType)
enum class EPGDamageType : uint8
{
    Physical,
    Magic,
    Fire,
    Ice,
    Lightning,
    Poison,
    Holy,
    Dark,
};
/**
 * RPG 메커닉을 위한 커스텀 데미지 타입 클래스
 */
UCLASS()
class UPLAYGROUND_API UPGDamageType : public UDamageType
{
    GENERATED_BODY()
    
public:
    UPGDamageType();
    
    
    // 주요 데미지 타입
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    EPGDamageType DamageType;
    
    // 추가 데미지 효과 변경자
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    bool bCanCritical;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    bool bIgnoresArmor;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
    bool bCausesStatusEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|StatusEffect")
    TSubclassOf<class UObject> StatusEffectClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|StatusEffect")
    float StatusEffectDuration;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|StatusEffect")
    float StatusEffectChance;
    
    // 데미지 효과
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Visuals")
    class UParticleSystem* HitEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Visuals")
    class UNiagaraSystem* NiagaraHitEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Visuals")
    class USoundBase* HitSound;
};
