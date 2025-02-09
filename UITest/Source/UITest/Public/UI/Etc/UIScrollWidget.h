// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "UIScrollWidget.generated.h"

class UListView;
class UTextBlock;

UCLASS(Meta=(DisableNativeTick))
class UITEST_API UUIScrollWIdgetItem : public UObject
{
	GENERATED_BODY()
public:
	int _data;
	int _index;
public:
	UUIScrollWIdgetItem() = default;
};

/**
 * 
 */
UCLASS()
class UITEST_API UUIScrollWidget : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* _txtIndex;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* _txtValue;

private:
	int32 _cachedValue = 0;
	
private:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	void SetValue(int32 InValue);
	int32 GetValue() const {return _cachedValue;}
};
