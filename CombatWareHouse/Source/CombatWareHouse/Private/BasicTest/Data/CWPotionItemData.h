// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTest/Data/CWItemData.h"
#include "CWPotionItemData.generated.h"

/**
 * 
 */
UCLASS()
class UCWPotionItemData : public UCWItemData
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = Hp)
	float _healAmount;

public:
	UCWPotionItemData();

public:
	FPrimaryAssetId GetPrimaryAssetId() const override;

public:
	FORCEINLINE float GetHealAmount() const { return _healAmount; }
};
