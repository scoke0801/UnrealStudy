// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BasicTest/Data/CWCharacterStat.h"
#include "CWCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangeDelegate, float /* CurrentHp */);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCWCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FOnHpZeroDelegate _onHpZeroDelegate;
	FOnHpChangeDelegate _onHpChangedDelegate;

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
	FCWCharacterStat _moditierStat;

public:	
	// Sets default values for this component's properties
	UCWCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	void SetHp(float InNewHp);

public:
	void SetLevelStat(int32 InNewLevel);

	FORCEINLINE float GetCurrentLevel() const { return _currentLevel; }
	FORCEINLINE void SetModifierStat(const FCWCharacterStat& InModifierStat) { _moditierStat = InModifierStat; }
	FORCEINLINE FCWCharacterStat GetTotalStat() const { return _baseStat + _moditierStat; }
	FORCEINLINE float GetCurrentHp() const { return _currentHp; }
	FORCEINLINE float GetAttackRadius() const { return _attackRadius; }

	float ApplyDamage(float InDamage);
};
