// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicTest/Data/CWCharacterStat.h"
#include "CWCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangeDelegate, float /* CurrentHp */); 
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FCWCharacterStat& /*BaseStat*/, const FCWCharacterStat& /*ModifierStat*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCWCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHpZeroDelegate _onHpZeroDelegate;
	FOnHpChangeDelegate _onHpChangedDelegate;
	FOnStatChangedDelegate _onStatChangedDeletage;

protected:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float _currentHp;
	
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float _currentLevel;

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float _attackRadius;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FCWCharacterStat _baseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FCWCharacterStat _modifierStat;

public:	
	// Sets default values for this component's properties
	UCWCharacterStatComponent();

protected: 
	virtual void InitializeComponent() override;
private:
	void SetHp(float InNewHp);

public:
	void SetLevelStat(int32 InNewLevel);

	FORCEINLINE const FCWCharacterStat& GetBaseStat() const { return _baseStat; }
	FORCEINLINE const FCWCharacterStat& GetModifierStat() const { return _modifierStat; }
	FORCEINLINE float GetCurrentLevel() const { return _currentLevel; }
	FORCEINLINE FCWCharacterStat GetTotalStat() const { return _baseStat + _modifierStat; }
	FORCEINLINE float GetCurrentHp() const { return _currentHp; }
	FORCEINLINE float GetAttackRadius() const { return _attackRadius; }

	void SetBaseStat(const FCWCharacterStat& InBaseStat);
	void SetModifierStat(const FCWCharacterStat& InModifierStat);

	float ApplyDamage(float InDamage);
};
