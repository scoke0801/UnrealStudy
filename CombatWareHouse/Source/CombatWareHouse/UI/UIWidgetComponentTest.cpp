// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatWareHouse/UI/UIWidgetComponentTest.h"

void UUIWidgetComponentTest::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(OnUIUpdate.IsBound())
	{
		OnUIUpdate.Broadcast(DeltaTime);
	}
}

void UUIWidgetComponentTest::EnableTick()
{
	for(const FOnCheckIsCanEnable& CheckFunc : EnableCheckList)
	{
		if(false == CheckFunc.IsBound())
		{
			return;
		}

		if(false == CheckFunc.Execute())
		{
			return;
		}
	}

	SetVisibility(true);
}

void UUIWidgetComponentTest::DisableTick()
{
	for(const FOnCheckIsCanDisable& CheckFunc : DisableCheckList)
	{
		if(false == CheckFunc.IsBound())
		{
			return;
		}

		if(false == CheckFunc.Execute())
		{
			return;
		}
	}
	SetVisibility(false);
}
