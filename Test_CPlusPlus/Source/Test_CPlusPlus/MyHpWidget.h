// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEST_CPLUSPLUS_API UMyHpWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindHp(class UMyStatComponent* StatComp);

	void UpdateHp();
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;

	TWeakObjectPtr<class UMyStatComponent> LastStatComp;
};
