// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/UI/CWUIHpBar.h"
#include "Components/ProgressBar.h"
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

	if (_hpProgressBar)
	{
		_hpProgressBar->SetPercent(InHp / _maxHp);
	}
}
