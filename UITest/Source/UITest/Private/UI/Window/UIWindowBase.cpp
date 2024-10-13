// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Window/UIWindowBase.h"

UUIWindowBase::UUIWindowBase(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	_uiType = EUIType::Window;
}
