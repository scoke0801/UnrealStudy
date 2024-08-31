// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/UI/CWWidgetComponentBase.h"
#include "CWUserWIdgetBase.h"

void UCWWidgetComponentBase::InitWidget()
{
	Super::InitWidget();

	// 함수 싸이클 상, 해당 함수가 호출되는 시점에는 액터가 생성되어 있다.
	// UWidgetComponent::BeginPlay에서 호출된다.

	/*
	BeginPlay전에 호출된 UComponent::SetWidgetClass 함수에서 위젯 클래스 정보를 설정하고,
	BeginPlay에서 실제 위젯을 생성?
	*/
	if (UCWUserWIdgetBase* UserWidget = Cast<UCWUserWIdgetBase>(GetWidget()))
	{
		UserWidget->SetOwningActor(GetOwner());
	}
}
