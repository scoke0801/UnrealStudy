// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ABPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);
/**
 * 
 */
UCLASS()
class ARENABATTLE_API AABPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AABPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	float GetExpRatio() const;
	int32 GetGameHighScore() const;
	int32 GetCharacterIndex() const;

	FString SaveSlotName;

	bool AddExp(int32 IncomeExp);
	void AddGameScore();

	void InitPlayerData();
	void SavePlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 Exp;

	UPROPERTY(Transient)
	int32 GameHighScore;

	UPROPERTY(Transient)
	int32 CharacterIndex;

private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FABCharacterData* CurrentStatData;
};