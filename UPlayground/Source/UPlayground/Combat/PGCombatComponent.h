#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PGCombatComponent.generated.h"

/**
 * 전투에 참여할 수 있는 엔티티를 위한 컴포넌트
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UPLAYGROUND_API UPGCombatComponent : public UActorComponent
{
    GENERATED_BODY()
    
public:
    UPGCombatComponent();
    
    // 기본 공격
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool PrimaryAttack();
    
    UFUNCTION(BlueprintCallable, Category = "Combat")
    bool SecondaryAttack();
    
    // 타게팅
    UFUNCTION(BlueprintCallable, Category = "Combat|Targeting")
    void SetTarget(AActor* NewTarget);
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Targeting")
    AActor* GetTarget() const;
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Targeting")
    void ClearTarget();
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Targeting")
    void SelectTarget();
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Targeting")
    TArray<AActor*> FindPotentialTargets(float Range, int32 MaxTargets);
    
    // 전투 스탯
    UFUNCTION(BlueprintCallable, Category = "Combat|Stats")
    float GetAttackPower() const;
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Stats")
    float GetDefensePower() const;
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Stats")
    float GetCriticalChance() const;
    
    // 전투 스탯 설정
    UFUNCTION(BlueprintCallable, Category = "Combat|Stats")
    void SetAttackPower(float NewValue);
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Stats")
    void SetDefensePower(float NewValue);
    
    UFUNCTION(BlueprintCallable, Category = "Combat|Stats")
    void SetCriticalChance(float NewValue);
    
    // 데미지 처리
    UFUNCTION(BlueprintCallable, Category = "Combat")
    virtual float ApplyDamage(AActor* Target, float DamageAmount, TSubclassOf<class UDamageType> DamageType);
    
protected:
    // 전투 상태
    UPROPERTY(ReplicatedUsing = OnRep_Target)
    AActor* TargetActor;
    
    // 타겟 복제 콜백
    UFUNCTION()
    void OnRep_Target();
    
    // 공격 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Attacks")
    class UAnimMontage* PrimaryAttackMontage;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Attacks")
    class UAnimMontage* SecondaryAttackMontage;
    
    // 전투 스탯
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Combat|Stats")
    float AttackPower;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Combat|Stats")
    float DefensePower;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Combat|Stats")
    float CriticalChance;
    
    // 공격 설정
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Settings")
    float AttackRange;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Settings")
    float AttackRadius;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Settings")
    float PrimaryAttackDamage;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Settings")
    float SecondaryAttackDamage;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat|Settings")
    float AttackCooldown;
    
    // 쿨다운 추적
    UPROPERTY()
    float AttackCooldownRemaining;
    
    FTimerHandle AttackCooldownTimerHandle;
    
    // 컴포넌트 라이프사이클
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // 헬퍼 메서드
    bool CanAttack() const;
    void StartAttackCooldown();
    
    UFUNCTION()
    void OnAttackCooldownEnd();
    
    // 공격 구현
    bool PerformAttack(float Damage, class UAnimMontage* AttackMontage);
    
    // 몽타주 재생
    void PlayAttackMontage(class UAnimMontage* Montage, FName Section = NAME_None);
    
    // 몽타주 노티파이 처리
    UFUNCTION()
    void OnAttackHit(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
    
    // 애니메이션 인스턴스 획득
    class UAnimInstance* GetOwnerAnimInstance() const;
    
    // 델리게이트
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetChanged, AActor*, NewTarget);
    
public:
    UPROPERTY(BlueprintAssignable, Category = "Combat|Targeting")
    FOnTargetChanged OnTargetChanged;
};
