// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PGIManager.h"
#include "PGGameStateTypes.h"
#include "PGBaseManager.generated.h"

/**
 * 기본 매니저 클래스
 * 모든 구체적인 매니저 클래스의 기본 구현을 제공합니다.
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
class UPLAYGROUND_API UPGBaseManager : public UObject, public IPGManager
{
	GENERATED_BODY()

public:
	UPGBaseManager();

	// IPGManager 인터페이스 구현
	virtual bool Initialize() override;
	virtual void Shutdown() override;
	virtual bool IsActive() const override;
	virtual FName GetManagerName() const override;
	virtual void Update(float DeltaTime) override;
	virtual void OnGameStateChanged(EPGGameStateType NewState) override;

protected:
	/** 매니저가 활성화되어 있는지 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Manager")
	bool bIsActive;

	/** 매니저의 고유 이름 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Manager")
	FName ManagerName;

	/** 매니저가 반응해야 하는 게임 상태 목록 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Manager")
	TArray<EPGGameStateType> ReactToGameStates;

	/** 현재 게임 상태 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Manager")
	EPGGameStateType CurrentGameState;

	/** 
	 * 특정 게임 상태에서 매니저가 활성화되어야 하는지 확인
	 * @param GameState 확인할 게임 상태
	 * @return 해당 상태에서 활성화되어야 하는지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager")
	bool ShouldBeActiveInState(EPGGameStateType GameState) const;

	/**
	 * 매니저의 구체적인 초기화 로직
	 * 자식 클래스에서 오버라이드하여 구현합니다.
	 * @return 초기화 성공 여부
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Manager")
	bool OnInitialize();
	virtual bool OnInitialize_Implementation();

	/**
	 * 매니저의 구체적인 종료 로직
	 * 자식 클래스에서 오버라이드하여 구현합니다.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Manager")
	void OnShutdown();
	virtual void OnShutdown_Implementation();

	/**
	 * 매니저의 구체적인 업데이트 로직
	 * 자식 클래스에서 오버라이드하여 구현합니다.
	 * @param DeltaTime 이전 프레임과의 시간 간격
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Manager")
	void OnUpdate(float DeltaTime);
	virtual void OnUpdate_Implementation(float DeltaTime);

	/**
	 * 게임 상태 변경 시 호출되는 구체적인 로직
	 * 자식 클래스에서 오버라이드하여 구현합니다.
	 * @param NewState 새로운 게임 상태
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Manager")
	void OnGameStateChange(EPGGameStateType NewState);
	virtual void OnGameStateChange_Implementation(EPGGameStateType NewState);
};
