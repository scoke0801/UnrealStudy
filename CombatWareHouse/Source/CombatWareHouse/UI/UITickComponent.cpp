// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatWareHouse/UI/UITickComponent.h"

void UUITickComponent::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(OnUIUpdate.IsBound())
	{
		OnUIUpdate.Broadcast(InDeltaTime);
	}
}

void UUITickComponent::EnableTick()
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

	SetVisibility(ESlateVisibility::SelfHitTestInvisible);
}

void UUITickComponent::DisableTick()
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
	
	SetVisibility(ESlateVisibility::Collapsed);
}
