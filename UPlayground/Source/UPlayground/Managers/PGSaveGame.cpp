// Copyright (c) 2025. All Rights Reserved.

#include "PGSaveGame.h"

UPGSaveGame::UPGSaveGame()
{
	// 기본값 초기화
	PlayerHealth = 100.0f;
	PlayerMaxHealth = 100.0f;
	PlayerLevel = 1;
	PlayerExperience = 0;
	InventoryCapacity = 20;
}

void UPGSaveGame::ResetSaveData()
{
	PlayerLocation = FVector::ZeroVector;
	PlayerRotation = FRotator::ZeroRotator;
	
	PlayerHealth = 100.0f;
	PlayerMaxHealth = 100.0f;
	PlayerLevel = 1;
	PlayerExperience = 0;
	
	InventoryItems.Empty();
	EquippedItems.Empty();
	InventoryCapacity = 20;
	
	ActiveQuests.Empty();
	CompletedQuests.Empty();
	FailedQuests.Empty();
	
	GameOptions.Empty();
	WorldStateFlags.Empty();
	CustomSaveData.Empty();
}

void UPGSaveGame::UpdateMetaData(const FString& InCharacterName, int32 InCharacterLevel, float InPlayTime, const FString& InCurrentLevel)
{
	MetaData.CharacterName = InCharacterName;
	MetaData.CharacterLevel = InCharacterLevel;
	MetaData.PlayTime = InPlayTime;
	MetaData.CurrentLevel = InCurrentLevel;
	MetaData.SaveDateTime = FDateTime::Now();
	MetaData.SaveLocation = PlayerLocation;
	MetaData.SaveRotation = PlayerRotation;
}

void UPGSaveGame::AddCustomData(const FString& Key, const FString& Value)
{
	CustomSaveData.Add(Key, Value);
}

bool UPGSaveGame::GetCustomData(const FString& Key, FString& OutValue) const
{
	if (const FString* Value = CustomSaveData.Find(Key))
	{
		OutValue = *Value;
		return true;
	}
	
	return false;
}
