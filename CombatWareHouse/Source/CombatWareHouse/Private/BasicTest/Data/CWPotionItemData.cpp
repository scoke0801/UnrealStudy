// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Data/CWPotionItemData.h"

UCWPotionItemData::UCWPotionItemData()
{
	_type = EItemType::Potion;
}

FPrimaryAssetId UCWPotionItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("CWItemData", GetFName());
}