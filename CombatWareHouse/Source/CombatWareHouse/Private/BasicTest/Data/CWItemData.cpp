// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Data/CWItemData.h"

FPrimaryAssetId UCWItemData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("CWItemData", GetFName());
}
