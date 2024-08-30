// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWComboAttackData.generated.h"

/**
 * 
 */
UCLASS()
class UCWComboAttackData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UCWComboAttackData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString _montageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 _maxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float _frameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> _effectiveFrameCount;
};
