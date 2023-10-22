// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHpWidget.h"
#include "MyStatComponent.h"
#include "Components/ProgressBar.h"

void UMyHpWidget::BindHp(UMyStatComponent* StatComp)
{
	//Meta�� BindWidget�� �ƴ� ���...
	// PB_HpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("Name..."))));

	LastStatComp = StatComp;

	StatComp->OnHpChanged.AddUObject(this, &UMyHpWidget::UpdateHp);
}

void UMyHpWidget::UpdateHp()
{
	if (LastStatComp.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Hp: %d, MaxHp: %d, Ratio: %f"),
			LastStatComp->GetHp(), LastStatComp->GetMaxHp(), LastStatComp->GetHpRatio());
		PB_HpBar->SetPercent(LastStatComp->GetHpRatio());
	}
}
