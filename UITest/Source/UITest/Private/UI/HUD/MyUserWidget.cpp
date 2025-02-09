// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MyUserWidget.h"

#include "IDetailTreeNode.h"
#include "UI/Etc/UIScrollWidget.h"
#include "Components/ListView.h"

void UMyUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	_listView->OnListViewScrolled().AddUObject(this, &UMyUserWidget::OnListViewScrolledInternal);
}
	
void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PopulateList();
}

void UMyUserWidget::OnListViewScrolledInternal(float InOffset, float DistanceRemaining)
{
	int32 Offset = InOffset;
	UE_LOG(LogTemp,Display, TEXT("OnListViewScrolledInternal - InOffset: %f, Offset: %d"), InOffset, Offset);
	if (IsLeftScrollEnd(InOffset))
	{
		HandleLeftScroll();
	}
	else if (IsRightScrollEnd(InOffset))
	{
		HandleRightScroll();
	}
}

void UMyUserWidget::PopulateList()
{
	Items.Empty();

	// 초기 아이템 생성
	for (int i = 1; i <= 30; ++i)
	{
		UUIScrollWIdgetItem* newItem = NewObject<UUIScrollWIdgetItem>(this);
		newItem->_index = i;
		newItem->_data = i;
		Items.Add(newItem);
	}
	_listView->SetListItems(Items);
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	_listView->SetScrollOffset(15);

	_cachedOffset = _listView->GetScrollOffset();
	UE_LOG(LogTemp,Display, TEXT("NativeConstruct - Offset: %f"), _listView->GetScrollOffset());
}

bool UMyUserWidget::IsLeftScrollEnd(float InOffset)
{
	return InOffset <= _visibleCount;
}

void UMyUserWidget::HandleLeftScroll()
{
	UE_LOG(LogTemp,Display, TEXT("HandleLeftScroll"));

	if (UObject* item = _listView->GetItemAt(_visibleCount))
	{
		if (UUIScrollWidget* Widget = Cast<UUIScrollWidget>(_listView->GetEntryWidgetFromItem(item)))
		{
			_cachedOffset = Widget->GetValue();
		}
	}

	UpdatePage();
	_listView->SetScrollOffset(15);
}

bool UMyUserWidget::IsRightScrollEnd(float InOffset)
{
	int32 DisplayedEndIndex = _listView->GetDisplayedEntryWidgets().Num() - 1;
	
	return (InOffset + DisplayedEndIndex >= Items.Num() - _visibleCount);
}

void UMyUserWidget::HandleRightScroll()
{
	UE_LOG(LogTemp,Display, TEXT("HandleRightScroll"));
	if (UObject* item = _listView->GetItemAt(Items.Num() - _visibleCount))
	{
		if (UUIScrollWidget* Widget = Cast<UUIScrollWidget>(_listView->GetEntryWidgetFromItem(item)))
		{
			_cachedOffset = Widget->GetValue();
		}
	}

	UpdatePage();
	_listView->SetScrollOffset(5);
}

void UMyUserWidget::UpdatePage()
{
	const TArray<UObject*> itemList = _listView->GetListItems();

	int index = 1;
	for (int32 i = _visibleCount; i < Items.Num() - _visibleCount; ++i, ++index)
	{
		int32 value = Wrap(_cachedOffset + index, _visibleCount, Items.Num() - _visibleCount);

		UE_LOG(LogTemp,Display, TEXT("UpdatePage - [%d] - value: [%d], index: [%d]"), i, value, index);
		if (UUIScrollWIdgetItem* data = Cast<UUIScrollWIdgetItem>(itemList[i]))
		{
			data->_index = i;
			data->_data = value;	
		}
		else
		{
			UE_LOG(LogTemp,Display, TEXT("UpdatePage fail"));
		}
	}
}

int32 UMyUserWidget::Wrap(int32 InValue, int32 InMin, int32 InMax) const
{
	int32 newValue = (InValue - InMin) % (InMax - InMin);
	return (newValue >= 0) ? (newValue + InMin) : (newValue+ InMax);
}
