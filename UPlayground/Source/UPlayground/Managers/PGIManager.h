// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGGameStateTypes.h"
#include "UObject/Interface.h"
#include "PGIManager.generated.h"

/**
 * 기본 매니저 인터페이스
 * 모든 게임 내 매니저 시스템은 이 인터페이스를 구현해야 합니다.
 */
UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UPGManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * 매니저 인터페이스 - 모든 게임 매니저의 기본 기능 정의
 */
class UPLAYGROUND_API IPGManager
{
	GENERATED_BODY()

public:
	/**
	 * 매니저 초기화 메서드
	 * 매니저가 처음 생성될 때 호출됩니다.
	 * @return 초기화 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager")
	virtual bool Initialize() = 0;

	/**
	 * 매니저 종료 메서드
	 * 매니저가 소멸되기 전에 호출됩니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager")
	virtual void Shutdown() = 0;

	/**
	 * 매니저 활성화 상태 확인
	 * @return 매니저가 활성화되어 있는지 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager")
	virtual bool IsActive() const = 0;

	/**
	 * 매니저 이름 반환
	 * @return 매니저의 고유 이름
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager")
	virtual FName GetManagerName() const = 0;

	/**
	 * 매니저 업데이트 메서드
	 * 프레임마다 호출됩니다.
	 * @param DeltaTime 이전 프레임과의 시간 간격
	 */
	virtual void Update(float DeltaTime) = 0;

	/**
	 * 게임 상태가 변경될 때 호출되는 메서드
	 * @param NewState 새로운 게임 상태
	 */
	virtual void OnGameStateChanged(EPGGameStateType NewState) = 0;
};
