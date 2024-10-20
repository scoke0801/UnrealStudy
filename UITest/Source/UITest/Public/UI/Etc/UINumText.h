// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GameWidgetBase.h"
#include "UINumText.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class UITEST_API UUINumText : public UGameWidgetBase
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(OnRemoveNotifyDelegate, int32);

public:
	OnRemoveNotifyDelegate OnRemoveNotify;

protected:
	UPROPERTY(EditAnywhere, Category = "UI", Meta = (BindWIdget))
	UTextBlock* _valueText;

protected:
	virtual void NativeConstruct() override;
public:
	void SetValueText(const int32 InValue);

private:
	void DestroySelf();
};
