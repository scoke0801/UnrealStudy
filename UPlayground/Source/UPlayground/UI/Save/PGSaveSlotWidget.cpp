// Copyright (c) 2025. All Rights Reserved.

#include "PGSaveSlotWidget.h"

void UPGSaveSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPGSaveSlotWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UPGSaveSlotWidget::SetSlotData(const FPGSaveMetaData& InMetaData)
{
	MetaData = InMetaData;
	bIsEmptySlot = false;
	OnUpdateSlotInfo();
}

void UPGSaveSlotWidget::SetEmptySlot(const FString& SlotName)
{
	MetaData = FPGSaveMetaData();
	MetaData.SlotName = SlotName;
	bIsEmptySlot = true;
	OnUpdateSlotInfo();
}

void UPGSaveSlotWidget::HandleSlotClicked()
{
	OnSlotSelected.Broadcast();
}

void UPGSaveSlotWidget::HandleDeleteClicked()
{
	OnSlotDeleteRequested.Broadcast();
}

FString UPGSaveSlotWidget::FormatDateTime(const FDateTime& DateTime) const
{
	return DateTime.ToString(TEXT("%Y-%m-%d %H:%M"));
}

FString UPGSaveSlotWidget::FormatPlayTime(float Seconds) const
{
	int32 Hours = FMath::FloorToInt(Seconds / 3600.0f);
	int32 Minutes = FMath::FloorToInt(FMath::Fmod(Seconds, 3600.0f) / 60.0f);
	int32 RemainingSeconds = FMath::FloorToInt(FMath::Fmod(Seconds, 60.0f));
	
	return FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, RemainingSeconds);
}
