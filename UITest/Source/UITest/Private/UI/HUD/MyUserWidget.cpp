// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/MyUserWidget.h"

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
	for (int i = 1; i <= 15; ++i)
	{
		UUIScrollWIdgetItem* newItem = NewObject<UUIScrollWIdgetItem>(this);
		newItem->_data = i;
		Items.Add(newItem);
	}
	_listView->SetListItems(Items);
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	_listView->SetScrollOffset(5);

	_cachedOffset = _listView->GetScrollOffset();
	UE_LOG(LogTemp,Display, TEXT("NativeConstruct - Offset: %f"), _listView->GetScrollOffset());
}

void UMyUserWidget::HandleScroll(float ScrollOffset)
{
	// if (!_listView) return;
	//
	// if (ScrollOffset >= MaxScroll - 50.0f) // 리스트의 끝에 도달하면
	// {
	// 	// 처음 아이템을 다시 리스트 끝에 추가
	// 	for (int i = 0; i < 5; i++)
	// 	{
	// 		UUIScrollWIdgetItem* ClonedItem = NewObject<UUIScrollWIdgetItem>();
	// 		//ClonedItem->ItemName = Items[i]->ItemName;
	// 		Items.Add(ClonedItem);
	// 	}
	// 	_listView->SetListItems(Items);
	// }
	// else if (ScrollOffset <= 50.0f) // 리스트의 맨 위에 도달하면
	// {
	// 	// 마지막 아이템을 다시 리스트 앞쪽에 추가
	// 	for (int i = 0; i < 5; i++)
	// 	{
	// 		UUIScrollWIdgetItem* ClonedItem = NewObject<UUIScrollWIdgetItem>();
	// 		//ClonedItem->ItemName = Items.Last()->ItemName;
	// 		Items.Insert(ClonedItem, 0);
	// 	}
	// 	_listView->SetListItems(Items);
	// }
}

bool UMyUserWidget::IsLeftScrollEnd(float InOffset)
{
	return InOffset <= 0.0f;
}

void UMyUserWidget::HandleLeftScroll()
{
	UE_LOG(LogTemp,Display, TEXT("HandleLeftScroll"));
	_listView->SetScrollOffset(5);
}

bool UMyUserWidget::IsRightScrollEnd(float InOffset)
{
	int32 DisplayedEndIndex = _listView->GetDisplayedEntryWidgets().Num() - 1;
	
	return (InOffset + DisplayedEndIndex >= (Items.Num()));
}

void UMyUserWidget::HandleRightScroll()
{
	UE_LOG(LogTemp,Display, TEXT("HandleRightScroll"));
	_listView->SetScrollOffset(5);
}
