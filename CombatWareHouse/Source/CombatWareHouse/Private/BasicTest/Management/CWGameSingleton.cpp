// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicTest/Management/CWGameSingleton.h"

#include "BasicTest/Data/CWCharacterStat.h"

DEFINE_LOG_CATEGORY(LogCWGameSingleton);

UCWGameSingleton::UCWGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Game/Data/DataTable/DT_CharacterStat.DT_CharacterStat"));
	if (nullptr != DataTableRef.Object)
	{
		if (const UDataTable* DataTable = DataTableRef.Object)
		{
			TArray<uint8*> ValueArray;
			DataTable->GetRowMap().GenerateValueArray(ValueArray);
			Algo::Transform(ValueArray, _characterStatTable,
				[](uint8* Value)
				{
					return *reinterpret_cast<FCWCharacterStat*>(Value);
				}
			);
		}
	}

	_characterMaxLevel = _characterStatTable.Num();
	ensure(_characterMaxLevel > 0);


#if WITH_EDITOR
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(TEXT("/Game/Data/DataTable/DT_CharacterStat.DT_CharacterStat"));
	if (DataTable.Object)
	{
		TArray<FCWCharacterStat*> TableRowArr;
		DataTable.Object->GetAllRows< FCWCharacterStat>(TEXT("GetAllRows"), TableRowArr);

		for (auto row : TableRowArr)
		{

		}
	}
#endif
}

UCWGameSingleton& UCWGameSingleton::Get()
{
	if (UCWGameSingleton* Singleton = CastChecked<UCWGameSingleton>(GEngine->GameSingleton))
	{
		return *Singleton;
	}
	
	UE_LOG(LogCWGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UCWGameSingleton>();
}
