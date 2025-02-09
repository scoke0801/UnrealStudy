// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ETC/UIScrollWidget.h"
#include "Components/TextBlock.h"

void UUIScrollWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	SetIsFocusable(false);
	
	if (UUIScrollWIdgetItem* Item =  Cast<UUIScrollWIdgetItem >(ListItemObject))
	{
		_cachedValue = Item->_data;
		_txtIndex->SetText(FText::FromString(FString::FromInt(Item->_index)));
		_txtValue->SetText(FText::FromString(FString::FromInt(Item->_data)));
	}
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	UE_LOG(LogTemp, Display, TEXT("NativeOnListItemObjectSet"));
}

void UUIScrollWidget::SetValue(int32 InValue)
{
	_cachedValue = InValue;
	_txtValue->SetText(FText::FromString(FString::FromInt(InValue)));
}
