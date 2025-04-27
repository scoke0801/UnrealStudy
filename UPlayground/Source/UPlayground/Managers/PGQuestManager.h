// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGBaseManager.h"
#include "PGQuestTypes.h"
#include "PGSaveTypes.h"
#include "PGQuestManager.generated.h"

/**
 * 퀘스트 매니저
 * 게임 내 퀘스트 상태 및 진행을 관리합니다.
 */
UCLASS(Blueprintable)
class UPLAYGROUND_API UPGQuestManager : public UPGBaseManager, public IPGSavableInterface
{
	GENERATED_BODY()

public:
	UPGQuestManager();

	//~Begin UPGBaseManager interface
	virtual bool OnInitialize_Implementation() override;
	virtual void OnShutdown_Implementation() override;
	virtual void OnUpdate_Implementation(float DeltaTime) override;
	virtual void OnGameStateChange_Implementation(EPGGameStateType NewState) override;
	//~End UPGBaseManager interface

	/**
	 * 퀘스트 시작
	 * @param QuestID 시작할 퀘스트 ID
	 * @return 퀘스트 시작 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool StartQuest(FName QuestID);

	/**
	 * 퀘스트 완료
	 * @param QuestID 완료할 퀘스트 ID
	 * @return 퀘스트 완료 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool CompleteQuest(FName QuestID);

	/**
	 * 퀘스트 실패
	 * @param QuestID 실패한 퀘스트 ID
	 * @return 퀘스트 실패 처리 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool FailQuest(FName QuestID);

	/**
	 * 퀘스트 포기
	 * @param QuestID 포기할 퀘스트 ID
	 * @return 퀘스트 포기 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool AbandonQuest(FName QuestID);

	/**
	 * 퀘스트 목표 업데이트
	 * @param QuestID 업데이트할 퀘스트 ID
	 * @param ObjectiveID 업데이트할 목표 ID
	 * @param Progress 업데이트할 진행도
	 * @return 업데이트 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool UpdateQuestObjective(FName QuestID, FName ObjectiveID, int32 Progress);

	/**
	 * 퀘스트 상태 확인
	 * @param QuestID 확인할 퀘스트 ID
	 * @return 퀘스트 상태
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	EPGQuestState GetQuestStatus(FName QuestID) const;

	/**
	 * 퀘스트 정보 획득
	 * @param QuestID 획득할 퀘스트 ID
	 * @param OutQuestInfo 퀘스트 정보
	 * @return 퀘스트 정보 획득 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool GetQuestInfo(FName QuestID, FPGActiveQuest& OutQuestInfo) const;

	/**
	 * 활성 퀘스트 목록 획득
	 * @return 활성 퀘스트 ID 목록
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<FName> GetActiveQuests() const;

	/**
	 * 완료된 퀘스트 목록 획득
	 * @return 완료된 퀘스트 ID 목록
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<FName> GetCompletedQuests() const;

	/**
	 * 실패한 퀘스트 목록 획득
	 * @return 실패한 퀘스트 ID 목록
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	TArray<FName> GetFailedQuests() const;

	/**
	 * 퀘스트 상태 변경 이벤트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnPGQuestStatusChanged OnQuestStatusChanged;

	/**
	 * 퀘스트 목표 업데이트 이벤트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnPGQuestObjectiveUpdated OnQuestObjectiveUpdated;

	/**
	 * 퀘스트 완료 이벤트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnPGQuestCompleted OnQuestCompleted;

	/**
	 * 퀘스트 실패 이벤트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnPGQuestFailed OnQuestFailed;

	/**
	 * 퀘스트 보상 획득 이벤트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Quest")
	FOnPGQuestRewardClaimed OnQuestRewardClaimed;

protected:
	/** 퀘스트 데이터 맵 (퀘스트 ID -> 퀘스트 정보) */
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TMap<FName, FPGActiveQuest> QuestData;

	/** 활성 퀘스트 목록 */
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TArray<FName> ActiveQuests;

	/** 완료된 퀘스트 목록 */
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TArray<FName> CompletedQuests;

	/** 실패한 퀘스트 목록 */
	UPROPERTY(VisibleAnywhere, Category = "Quest")
	TArray<FName> FailedQuests;

	/** 자동 저장 타이머 */
	float SaveTimer;

	/** 자동 저장 간격 (초) */
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	float AutoSaveInterval;

	/**
	 * 퀘스트 데이터 저장
	 * @return 저장 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool SaveQuestData();

	/**
	 * 퀘스트 데이터 로드
	 * @return 로드 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool LoadQuestData();

	/**
	 * 퀘스트 보상 제공
	 * @param QuestID 퀘스트 ID
	 * @return 보상 제공 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Quest")
	bool GiveQuestRewards(FName QuestID);

	/**
	 * 퀘스트 상태 변경
	 * @param QuestID 퀘스트 ID
	 * @param NewStatus 새 퀘스트 상태
	 * @return 상태 변경 성공 여부
	 */
	bool SetQuestStatus(FName QuestID, EPGQuestState NewStatus);

	//~Begin IPGSavableInterface
	virtual bool SaveData_Implementation(class UPGSaveGame* SaveGame) override;
	virtual bool LoadData_Implementation(class UPGSaveGame* SaveGame) override;
	//~End IPGSavableInterface
};
