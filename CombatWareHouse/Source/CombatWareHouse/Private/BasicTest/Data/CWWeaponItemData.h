// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasicTest/Data/CWItemData.h"
#include "CWWeaponItemData.generated.h"

/**
 * 
 */
UCLASS()
class UCWWeaponItemData : public UCWItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	TSoftObjectPtr<USkeletalMesh> _weaponMesh;
	

public:
	//FPrimaryAssetId GetPrimaryAssetId() const override;
};
