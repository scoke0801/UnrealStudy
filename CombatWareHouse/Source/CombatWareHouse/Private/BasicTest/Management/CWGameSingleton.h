// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "BasicTest/Data/CWCharacterStat.h"

#include "CWGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogCWGameSingleton, Error, All);
/**
 * 
 */
UCLASS()
class UCWGameSingleton : public UObject
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	int32 _characterMaxLevel;

private:
	TArray<FCWCharacterStat> _characterStatTable;

public:
	UCWGameSingleton();
	static UCWGameSingleton& Get();

public: 
	FORCEINLINE FCWCharacterStat GetCharacterStat(int32 InLevel) const { return _characterStatTable.IsValidIndex(InLevel - 1) ? _characterStatTable[InLevel - 1] : FCWCharacterStat(); }
	FORCEINLINE int32 GetChracterMaxLevel() const { return _characterMaxLevel; }

};
