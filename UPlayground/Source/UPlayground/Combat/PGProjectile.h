#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PGProjectile.generated.h"

/**
 * 원거리 공격을 위한 발사체 액터
 */
UCLASS()
class UPLAYGROUND_API APGProjectile : public AActor
{
    GENERATED_BODY()
    
public:
    APGProjectile();
    
    // 구성 요소
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class USphereComponent* CollisionComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UProjectileMovementComponent* ProjectileMovement;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UParticleSystemComponent* ParticleComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UAudioComponent* AudioComponent;
    
    // 발사체 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float Damage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    TSubclassOf<class UPGDamageType> DamageType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    float LifeSpan;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
    bool bDestroyOnHit;
    
    // 발사체 초기화
    UFUNCTION(BlueprintCallable, Category = "Projectile")
    void InitializeProjectile(AActor* ProjectileOwner, AActor* TargetActor, float InDamage, TSubclassOf<class UPGDamageType> InDamageType);
    
    // 물리 충돌 설정
    virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
    
protected:
    // 생명주기 함수
    virtual void BeginPlay() override;
    
    // 발사체 소유자 및 타겟
    UPROPERTY()
    AActor* InstigatorActor;
    
    UPROPERTY()
    AActor* TargetActor;
    
    // 히트 처리
    UFUNCTION()
    void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
    
    // 히트 효과
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    class UParticleSystem* ImpactEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    class UNiagaraSystem* NiagaraImpactEffect;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
    class USoundBase* ImpactSound;
};
