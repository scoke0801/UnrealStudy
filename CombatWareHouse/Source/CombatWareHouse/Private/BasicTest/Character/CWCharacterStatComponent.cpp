// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Character/CWCharacterStatComponent.h"

// Sets default values for this component's properties
UCWCharacterStatComponent::UCWCharacterStatComponent()
{
	// ...
	_maxHp = 200.0f;
}


// Called when the game starts
void UCWCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SetHp(_maxHp);
}

void UCWCharacterStatComponent::SetHp(float InNewHp)
{
	_currentHp = FMath::Clamp<float>(InNewHp, 0.0f, _maxHp);

	_onHpChangedDelegate.Broadcast(_currentHp);
}

float UCWCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = _currentHp;

	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (_currentHp <= KINDA_SMALL_NUMBER)
	{
		_onHpZeroDelegate.Broadcast();
	}

	return ActualDamage;
}

