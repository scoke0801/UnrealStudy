// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Popup/UIPopupBase.h"

UUIPopupBase::UUIPopupBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	_uiType = EUIType::Popup;
}
