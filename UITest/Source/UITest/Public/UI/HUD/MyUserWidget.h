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
	
private:
	TArray<class UUIScrollWIdgetItem*> Items;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;

	
	void OnListViewScrolledInternal(float InOffset, float DistanceRemaining);

	void PopulateList();
	void HandleScroll(float ScrollOffset);
};
