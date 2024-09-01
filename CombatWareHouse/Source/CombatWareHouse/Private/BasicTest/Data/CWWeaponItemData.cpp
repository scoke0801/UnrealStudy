// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Data/CWWeaponItemData.h"



UCWWeaponItemData::UCWWeaponItemData()
{
	_type = EItemType::Weapon;
}
FPrimaryAssetId UCWWeaponItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("CWItemData", GetFName());
}