// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CWUserWIdgetBase.h"
#include "BasicTest/Data/CWCharacterStat.h"
#include "CWUIHpBar.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS(meta = (DisableNativeTick))
class UCWUIHpBar : public UCWUserWIdgetBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = Ui, meta = (BindWidgetOptinal))
	TObjectPtr<UProgressBar> _hpProgressBar;

	UPROPERTY(EditAnywhere, Category = Ui, meta = (BindWidgetOptinal))
	TObjectPtr<UTextBlock> _txtHp;

	UPROPERTY()
	float _maxHp;

	UPROPERTY()
	float _currentHp;

public:
	UCWUIHpBar(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateHpBar(float InHp);

	void UpdateStat(const FCWCharacterStat& InBaseStat, const FCWCharacterStat& InModifierStat);

	FString GetHpStatText();
};
