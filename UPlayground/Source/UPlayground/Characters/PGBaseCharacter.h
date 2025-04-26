#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Equipment/PGEquipmentTypes.h"
#include "PGBaseCharacter.generated.h"

// 캐릭터 상태 enum
UENUM(BlueprintType)
enum class EPGCharacterState : uint8
{
    Idle,
    Moving,
    Attacking,
    Casting,
    Stunned,
    Dead
};
    
/**
 * 모든 캐릭터의 기본 클래스
 */
UCLASS()
class UPLAYGROUND_API APGBaseCharacter : public ACharacter
{
    GENERATED_BODY()
    
public:
    APGBaseCharacter();
    
    // 상태 관련 함수
    UFUNCTION(BlueprintCallable, Category = "Character")
    EPGCharacterState GetCharacterState() const;
    
    UFUNCTION(BlueprintCallable, Category = "Character")
    void SetCharacterState(EPGCharacterState NewState);
    
    // 체력 관리
    UFUNCTION(BlueprintCallable, Category = "Character")
    float GetHealth() const;
    
    UFUNCTION(BlueprintCallable, Category = "Character")
    float GetMaxHealth() const;
    
    UFUNCTION(BlueprintCallable, Category = "Character")
    void SetHealth(float NewHealth);
    
    // 데미지 처리
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, 
                            AController* EventInstigator, AActor* DamageCauser) override;
    
    // 상호작용 메서드
    UFUNCTION(BlueprintCallable, Category = "Character|Interaction")
    virtual void Interact(APGBaseCharacter* Interactor);
    
    UFUNCTION(BlueprintCallable, Category = "Character|Interaction")
    virtual bool CanBeInteractedWith() const;
    
    // 장비 관련 함수
    UFUNCTION(BlueprintCallable, Category = "Character|Equipment")
    bool EquipItemFromInventory(class UPGEquipmentItem* Item);
    
    UFUNCTION(BlueprintCallable, Category = "Character|Equipment")
    bool UnequipItemToInventory(EPGEquipmentSlot Slot);
    
protected:
    // 캐릭터 상태
    UPROPERTY(ReplicatedUsing = OnRep_CharacterState, BlueprintReadOnly, Category = "Character")
    EPGCharacterState CurrentState;
    
    // 체력 속성
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated, Category = "Character|Health")
    float Health;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character|Health")
    float MaxHealth;
    
    // 상태 복제 함수
    UFUNCTION()
    void OnRep_CharacterState();
    
    // 컴포넌트 레퍼런스
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
    class UPGCombatComponent* CombatComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
    class UPGAbilityComponent* AbilityComponent;
    
    // 인벤토리 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
    class UPGInventoryComponent* InventoryComponent;
    
    // 장비 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
    class UPGEquipmentComponent* EquipmentComponent;
    
    // 장비 시각화 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
    class UPGEquipmentVisualizerComponent* EquipmentVisualizerComponent;
};
