// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/UI/CWUICharacterStat.h"
#include "Components/TextBlock.h"

void UCWUICharacterStat::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FCWCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("_txt%sBaseStat"), *PropIt->GetName());
		const FName TextModifierControlName = *FString::Printf(TEXT("_txt%sModifierStat"), *PropIt->GetName());

		if (UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName)))
		{
			_baseLookup.Add(PropKey, BaseTextBlock);
		}

		if (UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName)))
		{
			_modifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void UCWUICharacterStat::UpdateStat(const FCWCharacterStat& InBaseStat, const FCWCharacterStat& InModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FCWCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());

		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&InBaseStat, &BaseData);
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&InModifierStat, &ModifierData);

		UTextBlock** BaseTextBlockPtr = _baseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}

		UTextBlock** ModifierTextBlockPtr = _modifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
