#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PGAttackData.generated.h"

// 공격 형태 Enum
UENUM(BlueprintType)
enum class EPGAttackShape : uint8
{
    Melee,
    Projectile,
    Sphere,
    Cone,
    Line
};
/**
 * 공격 설정을 위한 데이터 에셋
 */
UCLASS()
class UPLAYGROUND_API UPGAttackData : public UDataAsset
{
    GENERATED_BODY()
    
public:
    UPGAttackData();
    
    // 공격 식별
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    FName AttackID;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
    FText AttackName;
    
    // 애니메이션
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Animation")
    class UAnimMontage* AttackMontage;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Animation")
    FName MontageSection;
    
    // 데미지 정보
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Damage")
    float BaseDamage;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Damage")
    TSubclassOf<class UPGDamageType> DamageType;
    
    // 공격 형태
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Shape")
    TEnumAsByte<ECollisionChannel> TraceChannel;
    
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Shape")
    EPGAttackShape AttackShape;
    
    // 공격 속성
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Properties")
    float Range;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Properties")
    float Radius;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Properties")
    float Angle;
    
    // 발사체 설정 (해당되는 경우)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Projectile")
    TSubclassOf<class APGProjectile> ProjectileClass;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Projectile")
    float ProjectileSpeed;
    
    // 이펙트
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Effects")
    class UParticleSystem* AttackEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Effects")
    class UNiagaraSystem* NiagaraAttackEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Effects")
    class USoundBase* AttackSound;
    
    // 게임플레이 속성
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Gameplay")
    float Cooldown;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Gameplay")
    bool bRequiresTarget;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Gameplay")
    bool bCanBeBlocked;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Gameplay")
    bool bCanBeEvaded;
};
