// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGBaseManager.h"
#include "PGSaveTypes.h"
#include "PGSaveManager.generated.h"

/**
 * 저장 매니저
 * 게임의 저장/불러오기 기능을 관리합니다.
 */
UCLASS(Blueprintable)
class UPLAYGROUND_API UPGSaveManager : public UPGBaseManager
{
	GENERATED_BODY()

public:
	UPGSaveManager();

	//~Begin UPGBaseManager interface
	virtual bool OnInitialize_Implementation() override;
	virtual void OnShutdown_Implementation() override;
	virtual void OnUpdate_Implementation(float DeltaTime) override;
	virtual void OnGameStateChange_Implementation(EPGGameStateType NewState) override;
	//~End UPGBaseManager interface

	/**
	 * 게임 저장
	 * @param SlotName 저장 슬롯 이름
	 * @param SlotType 저장 슬롯 타입
	 * @param bOverwrite 덮어쓰기 여부
	 * @return 저장 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SaveGame(const FString& SlotName, EPGSaveSlotType SlotType = EPGSaveSlotType::Manual, bool bOverwrite = true);

	/**
	 * 게임 로드
	 * @param SlotName 로드할 슬롯 이름
	 * @return 로드 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool LoadGame(const FString& SlotName);

	/**
	 * 저장 슬롯 삭제
	 * @param SlotName 삭제할 슬롯 이름
	 * @return 삭제 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool DeleteSaveSlot(const FString& SlotName);

	/**
	 * 퀵 세이브
	 * @return 저장 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool QuickSave();

	/**
	 * 퀵 로드
	 * @return 로드 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool QuickLoad();

	/**
	 * 자동 저장
	 * @return 저장 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool AutoSave();

	/**
	 * 모든 저장 슬롯 정보 가져오기
	 * @return 저장 슬롯 메타데이터 배열
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	TArray<FPGSaveMetaData> GetAllSaveSlots();

	/**
	 * 저장 슬롯 존재 여부 확인
	 * @param SlotName 확인할 슬롯 이름
	 * @return 슬롯 존재 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool DoesSaveSlotExist(const FString& SlotName) const;

	/**
	 * 저장 슬롯 메타데이터 가져오기
	 * @param SlotName 슬롯 이름
	 * @param OutMetaData 메타데이터
	 * @return 메타데이터 존재 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool GetSaveSlotMetaData(const FString& SlotName, FPGSaveMetaData& OutMetaData) const;

	/**
	 * 자동 저장 설정 가져오기
	 * @return 자동 저장 설정
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	FPGAutoSaveConfig GetAutoSaveConfig() const { return AutoSaveConfig; }

	/**
	 * 자동 저장 설정 변경
	 * @param NewConfig 새로운 자동 저장 설정
	 */
	UFUNCTION(BlueprintCallable, Category = "Save")
	void SetAutoSaveConfig(const FPGAutoSaveConfig& NewConfig);

	// 이벤트 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Save")
	FOnPGSaveCompleted OnSaveCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Save")
	FOnPGLoadCompleted OnLoadCompleted;

	UPROPERTY(BlueprintAssignable, Category = "Save")
	FOnPGSaveDeleted OnSaveDeleted;

	UPROPERTY(BlueprintAssignable, Category = "Save")
	FOnPGAutoSaveTriggered OnAutoSaveTriggered;

protected:
	// 자동 저장 설정
	UPROPERTY(EditDefaultsOnly, Category = "Save")
	FPGAutoSaveConfig AutoSaveConfig;

	// 저장 슬롯 메타데이터 캐시
	UPROPERTY(VisibleAnywhere, Category = "Save")
	TMap<FString, FPGSaveMetaData> SaveSlotMetaDataCache;

	// 자동 저장 타이머
	float AutoSaveTimer;

	// 퀵 세이브 슬롯 이름
	UPROPERTY(EditDefaultsOnly, Category = "Save")
	FString QuickSaveSlotName;

	// 현재 자동 저장 인덱스
	int32 CurrentAutoSaveIndex;

	/**
	 * 저장 파일 생성
	 * @param SlotName 슬롯 이름
	 * @param SlotType 슬롯 타입
	 * @return 생성된 SaveGame 객체
	 */
	UFUNCTION()
	class UPGSaveGame* CreateSaveGameObject(const FString& SlotName, EPGSaveSlotType SlotType);

	/**
	 * 저장 가능한 객체들에서 데이터 수집
	 * @param SaveGame 저장할 SaveGame 객체
	 * @return 수집 성공 여부
	 */
	bool CollectSaveData(class UPGSaveGame* SaveGame);

	/**
	 * 저장된 데이터를 객체들에 적용
	 * @param SaveGame 로드된 SaveGame 객체
	 * @return 적용 성공 여부
	 */
	bool ApplyLoadedData(class UPGSaveGame* SaveGame);

	/**
	 * 메타데이터 캐시 업데이트
	 */
	void UpdateMetaDataCache();

	/**
	 * 자동 저장 슬롯 정리 (오래된 슬롯 삭제)
	 */
	void CleanupAutoSaveSlots();

	/**
	 * 저장 슬롯 이름 생성
	 * @param SlotType 슬롯 타입
	 * @return 생성된 슬롯 이름
	 */
	FString GenerateSaveSlotName(EPGSaveSlotType SlotType);

	/**
	 * 플레이어 데이터 수집
	 * @param SaveGame 저장할 SaveGame 객체
	 * @return 수집 성공 여부
	 */
	bool CollectPlayerData(class UPGSaveGame* SaveGame);

	/**
	 * 플레이어 데이터 적용
	 * @param SaveGame 로드된 SaveGame 객체
	 * @return 적용 성공 여부
	 */
	bool ApplyPlayerData(class UPGSaveGame* SaveGame);

	/**
	 * 인벤토리 데이터 수집
	 * @param SaveGame 저장할 SaveGame 객체
	 * @return 수집 성공 여부
	 */
	bool CollectInventoryData(class UPGSaveGame* SaveGame);

	/**
	 * 인벤토리 데이터 적용
	 * @param SaveGame 로드된 SaveGame 객체
	 * @return 적용 성공 여부
	 */
	bool ApplyInventoryData(class UPGSaveGame* SaveGame);

	/**
	 * 퀘스트 데이터 수집
	 * @param SaveGame 저장할 SaveGame 객체
	 * @return 수집 성공 여부
	 */
	bool CollectQuestData(class UPGSaveGame* SaveGame);

	/**
	 * 퀘스트 데이터 적용
	 * @param SaveGame 로드된 SaveGame 객체
	 * @return 적용 성공 여부
	 */
	bool ApplyQuestData(class UPGSaveGame* SaveGame);
};
