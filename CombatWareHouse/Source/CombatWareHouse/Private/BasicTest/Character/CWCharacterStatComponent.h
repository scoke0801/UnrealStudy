// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float _maxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float _currentHp;

public:	
	// Sets default values for this component's properties
	UCWCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
private:
	void SetHp(float InNewHp);

public:
	FORCEINLINE float GetMaxHp() const { return _maxHp; }
	FORCEINLINE float GetCurrentHp() const { return _currentHp; }

	float ApplyDamage(float InDamage);
};
