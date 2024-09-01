// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicTest/Data/CWCharacterStat.h"
#include "CWUIHUDIngame.generated.h"

class UCWUIHpBar;
class UCWUICharacterStat;
/**
 * 
 */
UCLASS()
class UCWUIHUDIngame : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	TObjectPtr<UCWUIHpBar> _hpBar;
	
	UPROPERTY(VisibleAnywhere, Category = UI, meta = (BindWidget))
	TObjectPtr<UCWUICharacterStat> _characterStat;
public:
	UCWUIHUDIngame(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FCWCharacterStat& InBaseStat, const FCWCharacterStat& InModifierStat);
	void UpdateHpBar(float InNewHp);

protected:
	virtual void NativeConstruct() override;

};
