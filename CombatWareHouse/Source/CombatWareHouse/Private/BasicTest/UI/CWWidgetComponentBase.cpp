// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/UI/CWWidgetComponentBase.h"
#include "CWUserWIdgetBase.h"

void UCWWidgetComponentBase::InitWidget()
{
	Super::InitWidget();

	// 함수 싸이클 상, 해당 함수가 호출되는 시점에는 액터가 생성되어 있다.
	// UWidgetComponent::BeginPlay에서 호출된다.
	if (UCWUserWIdgetBase* UserWidget = Cast<UCWUserWIdgetBase>(GetWidget()))
	{
		UserWidget->SetOwningActor(GetOwner());
	}
}
