// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTest/Data/CWItemData.h"
#include "BasicTest/Data/CWCharacterStat.h"
#include "CWScrollItemData.generated.h"

/**
 * 
 */
UCLASS()
class UCWScrollItemData : public UCWItemData
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = Stat)
	FCWCharacterStat _baseStat;

public:
	UCWScrollItemData();

public:
	FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	FORCEINLINE const FCWCharacterStat& GetBaseStat() const { return _baseStat; }
	
};
