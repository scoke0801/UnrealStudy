#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PGCharacterMovementComponent.generated.h"

// 이동 상태
UENUM(BlueprintType)
enum class EPGMovementState : uint8
{
    Walking,
    Sprinting,
    Dodging,
    Stunned,
    Knocked
};
/**
 * RPG 캐릭터 이동을 위한 커스텀 무브먼트 컴포넌트
 */
UCLASS()
class UPLAYGROUND_API UPGCharacterMovementComponent : public UCharacterMovementComponent
{
    GENERATED_BODY()
    
public:
    UPGCharacterMovementComponent();
    
    // 상태 관련 함수
    UFUNCTION(BlueprintCallable, Category = "Movement")
    EPGMovementState GetMovementState() const;
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMovementState(EPGMovementState NewState);
    
    // 회피 기능
    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool CanDodge() const;
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Dodge(FVector Direction);
    
    // 달리기 기능
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StartSprinting();
    
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopSprinting();
    
protected:
    // 이동 상태
    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Movement")
    EPGMovementState CurrentMovementState;
    
    // 회피 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Dodge")
    float DodgeStrength;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Dodge")
    float DodgeDuration;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Dodge")
    float DodgeCooldown;
    
    // 달리기 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
    float SprintSpeedMultiplier;
    
    // 기본 걷기 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Sprint")
    float DefaultWalkSpeed;
    
    // 오버라이드
    virtual void PhysWalking(float deltaTime, int32 Iterations) override;
    virtual void PhysCustom(float deltaTime, int32 Iterations) override;
    
    // 복제 속성
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    // 회피 구현
    void PerformDodge(float DeltaTime);
    
    // 타이머 핸들
    FTimerHandle DodgeTimerHandle;
    FTimerHandle DodgeCooldownTimerHandle;
    
    // 회피 상태 추적
    bool bIsDodging;
    bool bDodgeOnCooldown;
    FVector DodgeDirection;
};
