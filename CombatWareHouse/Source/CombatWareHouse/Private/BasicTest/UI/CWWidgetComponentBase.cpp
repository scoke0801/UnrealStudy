// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/UI/CWWidgetComponentBase.h"
#include "CWUserWIdgetBase.h"

void UCWWidgetComponentBase::InitWidget()
{
	Super::InitWidget();

	// �Լ� ����Ŭ ��, �ش� �Լ��� ȣ��Ǵ� �������� ���Ͱ� �����Ǿ� �ִ�.
	// UWidgetComponent::BeginPlay���� ȣ��ȴ�.

	/*
	BeginPlay���� ȣ��� UComponent::SetWidgetClass �Լ����� ���� Ŭ���� ������ �����ϰ�,
	BeginPlay���� ���� ������ ����?
	*/
	if (UCWUserWIdgetBase* UserWidget = Cast<UCWUserWIdgetBase>(GetWidget()))
	{
		UserWidget->SetOwningActor(GetOwner());
	}
}
