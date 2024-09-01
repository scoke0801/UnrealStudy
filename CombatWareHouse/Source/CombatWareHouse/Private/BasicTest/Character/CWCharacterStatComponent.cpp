// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Character/CWCharacterStatComponent.h"
#include "BasicTest/Management/CWGameSingleton.h"

// Sets default values for this component's properties
UCWCharacterStatComponent::UCWCharacterStatComponent()
{
	_currentLevel = 1;
	_attackRadius = 50.0f;

	bWantsInitializeComponent = true;
} 

void UCWCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(_currentLevel);
	SetHp(_baseStat.MaxHp);
}

void UCWCharacterStatComponent::SetHp(float InNewHp)
{
	_currentHp = FMath::Clamp<float>(InNewHp, 0.0f, _baseStat.MaxHp);

	_onHpChangedDelegate.Broadcast(_currentHp);
}

void UCWCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	_currentLevel = FMath::Clamp(InNewLevel, 1, UCWGameSingleton::Get().GetChracterMaxLevel());
	_baseStat = UCWGameSingleton::Get().GetCharacterStat(_currentLevel);
}

void UCWCharacterStatComponent::HealHP(float InHealAmount)
{ 
	_currentHp = FMath::Clamp(_currentHp + InHealAmount, 0, GetTotalStat().MaxHp); 

	_onHpChangedDelegate.Broadcast(_currentHp);
}

void UCWCharacterStatComponent::AddBaseStat(const FCWCharacterStat& InAddBaseStat)
{
	_baseStat = _baseStat + InAddBaseStat;

	_onStatChangedDeletage.Broadcast(_baseStat, GetModifierStat());
}

void UCWCharacterStatComponent::SetBaseStat(const FCWCharacterStat& InBaseStat)
{
	_baseStat = InBaseStat; 

	_onStatChangedDeletage.Broadcast(GetBaseStat(), GetModifierStat());
}

void UCWCharacterStatComponent::SetModifierStat(const FCWCharacterStat& InModifierStat)
{
	_modifierStat = InModifierStat;

	_onStatChangedDeletage.Broadcast(GetBaseStat(), GetModifierStat());
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

