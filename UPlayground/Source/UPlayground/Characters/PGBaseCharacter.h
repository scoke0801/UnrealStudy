// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PGBaseCharacter.generated.h"

UCLASS(Abstract)
class UPLAYGROUND_API APGBaseCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    APGBaseCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    // 캐릭터 기본 속성
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
    float MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
    float CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
    float MaxMana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
    float CurrentMana;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Stats")
    float MovementSpeed;

    // 인벤토리 컴포넌트 참조
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    class UPGInventoryComponent* InventoryComponent;

    // 기본 캐릭터 메서드
    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void ApplyDamageToCharacter(float DamageAmount, AActor* DamageCauser);

    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void Die();

    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void Heal(float HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual bool IsAlive() const;

    UFUNCTION(BlueprintCallable, Category = "Character")
    virtual void LevelUp();

public:
    // 상호작용 시스템 - 외부에서 접근 가능하도록 public으로 변경
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual void Interact(APGBaseCharacter* Interactor);

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    virtual bool CanBeInteractedWith() const;

protected:

    // 델리게이트 선언
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCharacterDamaged, APGBaseCharacter*, DamagedCharacter, float, DamageAmount);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDied, APGBaseCharacter*, DeadCharacter);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterLevelUp, APGBaseCharacter*, Character);

    // 이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Character Events")
    FOnCharacterDamaged OnCharacterDamaged;

    UPROPERTY(BlueprintAssignable, Category = "Character Events")
    FOnCharacterDied OnCharacterDied;

    UPROPERTY(BlueprintAssignable, Category = "Character Events")
    FOnCharacterLevelUp OnCharacterLevelUp;
};