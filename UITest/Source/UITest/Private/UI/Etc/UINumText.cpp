// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Etc/UINumText.h"
#include "Components/TextBlock.h"

void UUINumText::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &UUINumText::DestroySelf, 1.2f);
}

void UUINumText::SetValueText(const int32 InValue)
{
	_valueText->SetText(FText::FromString(FString::FromInt(InValue)));
}

void UUINumText::DestroySelf()
{
	OnRemoveNotify.ExecuteIfBound(GetUniqueID());

	RemoveFromViewport();
}
