#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PGAbilityTypes.h"
#include "PGAbility.generated.h"

// 능력 상태
UENUM(BlueprintType)
enum class EPGAbilityState : uint8
{
    Inactive,
    Activating,
    Active,
    Cooldown
};
/**
 * 모든 능력의 기본 클래스
 */
UCLASS(Blueprintable)
class UPLAYGROUND_API UPGAbility : public UObject
{
    GENERATED_BODY()
    
public:
    UPGAbility();
    
    
    // 능력 식별
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    FName AbilityID;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    FText AbilityName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
    FText AbilityDescription;
    
    // 능력 시각 효과
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|UI")
    class UTexture2D* AbilityIcon;
    
    // 능력 요구사항
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Requirements")
    TMap<EPGResourceType, float> ResourceCosts;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Requirements")
    bool bRequiresTarget;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Requirements")
    float Range;
    
    // 쿨다운 속성
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability|Cooldown")
    float CooldownDuration;
    
    // 능력 활성화
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool CanActivate() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool Activate();
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    void End(bool bWasCancelled = false);
    
    // 상태 게터
    UFUNCTION(BlueprintCallable, Category = "Ability")
    EPGAbilityState GetAbilityState() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool IsActive() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    bool IsOnCooldown() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float GetCooldownTimeRemaining() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ability")
    float GetCooldownPercent() const;
    
protected:
    // 능력 소유자
    UPROPERTY()
    AActor* OwningActor;
    
    // 능력 컴포넌트 참조
    UPROPERTY()
    class UPGAbilityComponent* AbilityComponent;
    
    // 능력 상태
    UPROPERTY(ReplicatedUsing = OnRep_AbilityState)
    EPGAbilityState CurrentState;
    
    // 쿨다운 추적
    UPROPERTY(Replicated)
    float CooldownTimeRemaining;
    
    FTimerHandle CooldownTimerHandle;
    
    // 복제 속성
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
    
    // 상태 복제 콜백
    UFUNCTION()
    void OnRep_AbilityState();
    
    // 능력 구현
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
    void OnActivate();
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Ability")
    void OnEnd(bool bWasCancelled);
    
    // 네이티브 구현
    UFUNCTION(BlueprintNativeEvent, Category = "Ability")
    void K2_OnActivate();
    virtual void K2_OnActivate_Implementation();
    
    UFUNCTION(BlueprintNativeEvent, Category = "Ability")
    void K2_OnEnd(bool bWasCancelled);
    virtual void K2_OnEnd_Implementation(bool bWasCancelled);
    
    // 쿨다운 관리
    void StartCooldown();
    void ClearCooldown();
    
    UFUNCTION()
    void OnCooldownEnd();
    
    friend class UPGAbilityComponent;
};
