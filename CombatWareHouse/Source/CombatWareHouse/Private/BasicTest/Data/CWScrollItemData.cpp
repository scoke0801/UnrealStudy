// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Data/CWScrollItemData.h"

UCWScrollItemData::UCWScrollItemData()
{
	_type = EItemType::Scroll;
}

FPrimaryAssetId UCWScrollItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("CWItemData", GetFName());
}