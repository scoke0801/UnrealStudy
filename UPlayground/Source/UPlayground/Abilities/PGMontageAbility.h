#pragma once

#include "CoreMinimal.h"
#include "PGAbility.h"
#include "PGMontageAbility.generated.h"

/**
 * 애니메이션 몽타주를 재생하는 능력
 */
UCLASS()
class UPLAYGROUND_API UPGMontageAbility : public UPGAbility
{
    GENERATED_BODY()
    
public:
    UPGMontageAbility();
    
    // 재생할 애니메이션 몽타주
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    class UAnimMontage* AbilityMontage;
    
    // 재생할 애니메이션 섹션 (있는 경우)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    FName MontageSection;
    
    // 몽타주 종료시 능력 종료 여부
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    bool bEndAbilityWithMontage;
    
protected:
    // 몽타주 인스턴스 포인터
    UPROPERTY()
    class UAnimInstance* AnimInstance;
    
    // 현재 재생 중인 몽타주
    UPROPERTY()
    class UAnimMontage* CurrentMontageInstance;
    
    // 몽타주 종료 델리게이트 핸들
    FDelegateHandle MontageEndedDelegate;
    
    // 능력 라이프사이클 오버라이드
    virtual void K2_OnActivate_Implementation() override;
    virtual void K2_OnEnd_Implementation(bool bWasCancelled) override;
    
    // 몽타주 콜백
    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
    
    // 애님 노티파이 처리를 위한 이벤트
    UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
    void OnAbilityNotifyBegin(FName NotifyName);
    
    UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
    void OnAbilityNotifyEnd(FName NotifyName);
    
    // 노티파이 핸들러
    UFUNCTION()
    void HandleNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
    
    UFUNCTION()
    void HandleNotifyEnd(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);
};
