// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicTest/Data/CWCharacterStat.h"
#include "CWUICharacterStat.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class UCWUICharacterStat : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	TMap<FName, UTextBlock*> _baseLookup;

	UPROPERTY()
	TMap<FName, UTextBlock*> _modifierLookup;

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FCWCharacterStat& InBaseStat, const FCWCharacterStat& InModifierStat);
};
