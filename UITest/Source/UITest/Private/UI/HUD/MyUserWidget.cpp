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

	//if (UUIWindowBase* Widget = UI()->OpenWIdget<UUIWindowBase>(TEXT("WindowBase")))
	//{
	//}
	//
	
	for (int i = 1; i <= 30; ++i)
	{
		UUIScrollWIdgetItem* newItem = NewObject<UUIScrollWIdgetItem>(this);
		newItem->_data = i;
		Items.Add(newItem);
	}
	_listView->SetListItems(Items);
	_listView->SetScrollbarVisibility(ESlateVisibility::Collapsed);

	_listView->NavigateToIndex(Items.Num() * 0.5f);
	UE_LOG(LogTemp,Display, TEXT("NativeConstruct - Offset: %f"), _listView->GetScrollOffset());

	PopulateList();
}

void UMyUserWidget::OnListViewScrolledInternal(float InOffset, float DistanceRemaining)
{
	int32 Offset = InOffset;
	UE_LOG(LogTemp,Display, TEXT("OnListViewScrolledInternal - InOffset: %f, Offset: %d"), InOffset, Offset);
}

void UMyUserWidget::PopulateList()
{
	Items.Empty();

	// 초기 10개 아이템 생성
	for (int i = 0; i < 10; i++)
	{
		UUIScrollWIdgetItem* NewItem = NewObject<UUIScrollWIdgetItem>();
		//NewItem->ItemName = FString::Printf(TEXT("Item %d"), i);
		Items.Add(NewItem);
	}

	_listView->SetListItems(Items);
}

void UMyUserWidget::HandleScroll(float ScrollOffset)
{
	if (!_listView) return;

	float MaxScroll = Items.Num();

	if (ScrollOffset >= MaxScroll - 50.0f) // 리스트의 끝에 도달하면
	{
		// 처음 아이템을 다시 리스트 끝에 추가
		for (int i = 0; i < 5; i++)
		{
			UUIScrollWIdgetItem* ClonedItem = NewObject<UUIScrollWIdgetItem>();
			//ClonedItem->ItemName = Items[i]->ItemName;
			Items.Add(ClonedItem);
		}
		_listView->SetListItems(Items);
	}
	else if (ScrollOffset <= 50.0f) // 리스트의 맨 위에 도달하면
	{
		// 마지막 아이템을 다시 리스트 앞쪽에 추가
		for (int i = 0; i < 5; i++)
		{
			UUIScrollWIdgetItem* ClonedItem = NewObject<UUIScrollWIdgetItem>();
			//ClonedItem->ItemName = Items.Last()->ItemName;
			Items.Insert(ClonedItem, 0);
		}
		_listView->SetListItems(Items);
	}
}
