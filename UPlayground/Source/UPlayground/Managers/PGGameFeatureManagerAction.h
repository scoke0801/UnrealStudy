// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction.h"
#include "PGIManager.h"
#include "PGGameFeatureManagerAction.generated.h"

/**
 * 게임 피처 매니저 액션
 * 게임 피처 플러그인에서 매니저를 등록하고 관리하기 위한 액션
 */
UCLASS(BlueprintType, Blueprintable)
class UPLAYGROUND_API UPGGameFeatureManagerAction : public UGameFeatureAction
{
	GENERATED_BODY()

public:
	UPGGameFeatureManagerAction();
	virtual ~UPGGameFeatureManagerAction() override;

	/** 등록할 매니저 클래스 */
	UPROPERTY(EditAnywhere, Category = "Manager")
	TArray<TSubclassOf<UObject>> ManagerClasses;

	//~UGameFeatureAction interface
	virtual void OnGameFeatureRegistering() override;
	virtual void OnGameFeatureUnregistering() override;
	virtual void OnGameFeatureActivating() override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	//~End of UGameFeatureAction interface

private:
	/** 생성된 매니저 인스턴스 추적 */
	TMap<TWeakObjectPtr<UGameInstance>, TArray<TWeakObjectPtr<UObject>>> SpawnedManagers;

	/**
	 * 모든 게임 인스턴스에 매니저 추가
	 */
	void AddManagersToAllWorlds();

	/**
	 * 모든 게임 인스턴스에서 매니저 제거
	 */
	void RemoveManagersFromAllWorlds();

	/**
	 * 게임 인스턴스에 매니저 추가
	 * @param GameInstance 게임 인스턴스
	 */
	void AddManagersToGameInstance(UGameInstance* GameInstance);

	/**
	 * 게임 인스턴스에서 매니저 제거
	 * @param GameInstance 게임 인스턴스
	 */
	void RemoveManagersFromGameInstance(UGameInstance* GameInstance);

	/**
	 * 특정 게임 인스턴스에 매니저 등록
	 * @param GameInstance 게임 인스턴스
	 * @return 생성된 매니저 목록
	 */
	TArray<TWeakObjectPtr<UObject>> RegisterManagersForGameInstance(UGameInstance* GameInstance);
};
