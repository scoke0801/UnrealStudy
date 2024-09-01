// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/UI/CWUIHpBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/CWCharacterWidgetInterface.h"


UCWUIHpBar::UCWUIHpBar(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	,_maxHp(-1.0f)
{
}
void UCWUIHpBar::NativeConstruct()
{
	Super::NativeConstruct();

	// _hpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("_hpProgressBar")));
	ensure(_hpProgressBar);

	if (ICWCharacterWidgetInterface* CharacterWidget = Cast<ICWCharacterWidgetInterface>(_owningActor))
	{
		CharacterWidget->SetupCharacterWidget(this);
	}
}

void UCWUIHpBar::UpdateHpBar(float InHp)
{
	// ensure(InHp > 0.0f);
	_currentHp = InHp;

	if (_hpProgressBar)
	{
		_hpProgressBar->SetPercent(InHp / _maxHp);
	}

	if (_txtHp)
	{
		_txtHp->SetText(FText::FromString(GetHpStatText()));
	}
}

void UCWUIHpBar::UpdateStat(const FCWCharacterStat& InBaseStat, const FCWCharacterStat& InModifierStat)
{
	_maxHp = (InBaseStat + InModifierStat).MaxHp;

	if (_hpProgressBar)
	{
		_hpProgressBar->SetPercent(_currentHp / _maxHp);
	}
}

FString UCWUIHpBar::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%0.f"), _currentHp, _maxHp);
}
