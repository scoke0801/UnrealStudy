// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class UITEST_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "UI", Meta = (BindWidget))
	TObjectPtr<class UListView> _listView;


	UPROPERTY(EditAnywhere, Category = "UIValue")
	int32 _visibleCount =5;
	
private:
	TArray<class UUIScrollWIdgetItem*> Items;

	int _cachedOffset = 0;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	
	void OnListViewScrolledInternal(float InOffset, float DistanceRemaining);

	void PopulateList();
	void HandleScroll(float ScrollOffset);

	bool IsLeftScrollEnd(float InOffset);
	void HandleLeftScroll();
	
	bool IsRightScrollEnd(float InOffset);
	void HandleRightScroll();

	void UpdatePage();

private:
	int32 Wrap(int32 InValue, int32 InMin, int32 InMax) const;
};
