// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PGSaveTypes.h"
#include "PGInventoryTypes.h"
#include "PGQuestTypes.h"
#include "PGSaveGame.generated.h"

/**
 * 저장 데이터 컨테이너 클래스
 * 게임의 모든 저장 데이터를 포함합니다.
 */
UCLASS()
class UPLAYGROUND_API UPGSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPGSaveGame();

	//=== 메타데이터 ===//
	UPROPERTY(VisibleAnywhere, Category = "Save Meta")
	FPGSaveMetaData MetaData;

	//=== 플레이어 데이터 ===//
	
	// 플레이어 위치 및 회전
	UPROPERTY(VisibleAnywhere, Category = "Player")
	FVector PlayerLocation;
	
	UPROPERTY(VisibleAnywhere, Category = "Player")
	FRotator PlayerRotation;
	
	// 플레이어 스탯
	UPROPERTY(VisibleAnywhere, Category = "Player")
	float PlayerHealth;
	
	UPROPERTY(VisibleAnywhere, Category = "Player")
	float PlayerMaxHealth;
	
	// 플레이어 레벨 및 경험치
	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 PlayerLevel;
	
	UPROPERTY(VisibleAnywhere, Category = "Player")
	int32 PlayerExperience;

	//=== 인벤토리 데이터 ===//
	
	// 인벤토리 아이템 목록
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TMap<FName, int32> InventoryItems;
	
	// 장착 아이템 데이터
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TMap<FName, FName> EquippedItems;  // 슬롯 이름 -> 아이템 ID
	
	// 인벤토리 용량
	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	int32 InventoryCapacity;

	//=== 퀘스트 데이터 ===//
	
	// 활성 퀘스트 데이터
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TMap<FName, FPGActiveQuest> ActiveQuests;
	
	// 완료된 퀘스트 목록
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TArray<FName> CompletedQuests;
	
	// 실패한 퀘스트 목록
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TArray<FName> FailedQuests;

	//=== 기타 게임 데이터 ===//
	
	// 게임 옵션 설정
	UPROPERTY(VisibleAnywhere, Category = "Game Options")
	TMap<FString, FString> GameOptions;
	
	// 월드 상태 데이터
	UPROPERTY(VisibleAnywhere, Category = "World State")
	TMap<FName, bool> WorldStateFlags;
	
	// 커스텀 저장 데이터 (매니저별 추가 데이터)
	UPROPERTY(VisibleAnywhere, Category = "Custom Data")
	TMap<FString, FString> CustomSaveData;

	/**
	 * 저장 데이터 초기화
	 */
	void ResetSaveData();

	/**
	 * 메타데이터 업데이트
	 * @param InCharacterName 캐릭터 이름
	 * @param InCharacterLevel 캐릭터 레벨
	 * @param InPlayTime 플레이 시간
	 * @param InCurrentLevel 현재 레벨
	 */
	void UpdateMetaData(const FString& InCharacterName, int32 InCharacterLevel, float InPlayTime, const FString& InCurrentLevel);

	/**
	 * 커스텀 데이터 추가
	 * @param Key 데이터 키
	 * @param Value 데이터 값
	 */
	void AddCustomData(const FString& Key, const FString& Value);

	/**
	 * 커스텀 데이터 획득
	 * @param Key 데이터 키
	 * @param OutValue 데이터 값
	 * @return 데이터 존재 여부
	 */
	bool GetCustomData(const FString& Key, FString& OutValue) const;
};
