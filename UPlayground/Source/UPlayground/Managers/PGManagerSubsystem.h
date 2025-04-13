// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PGIManager.h"
#include "PGGameStateTypes.h"
#include "Tickable.h"
#include "PGManagerSubsystem.generated.h"

/**
 * 매니저 서브시스템
 * 게임 내 모든 매니저를 관리하고 조정합니다.
 */
UCLASS()
class UPLAYGROUND_API UPGManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPGManagerSubsystem();

	// USubsystem 인터페이스 구현
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	/** 
	 * 매니저 등록 메서드
	 * @param Manager 등록할 매니저 인스턴스
	 * @return 등록 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager Subsystem")
	bool RegisterManager(UObject* Manager);

	/**
	 * 매니저 해제 메서드
	 * @param ManagerName 해제할 매니저의 이름
	 * @return 해제 성공 여부
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager Subsystem")
	bool UnregisterManager(FName ManagerName);

	/**
	 * 특정 타입의 매니저 찾기
	 * @tparam T 찾을 매니저 타입
	 * @return 찾은 매니저 인스턴스, 없으면 nullptr
	 */
	template<class T>
	T* GetManager() const;

	/**
	 * 이름으로 매니저 찾기
	 * @param ManagerName 찾을 매니저의 이름
	 * @return 찾은 매니저 인스턴스, 없으면 nullptr
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager Subsystem")
	UObject* GetManagerByName(FName ManagerName) const;

	/**
	 * 게임 상태 설정
	 * @param NewState 새로운 게임 상태
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager Subsystem")
	void SetGameState(EPGGameStateType NewState);

	/**
	 * 현재 게임 상태 반환
	 * @return 현재 게임 상태
	 */
	UFUNCTION(BlueprintCallable, Category = "Manager Subsystem")
	EPGGameStateType GetCurrentGameState() const;

	/**
	 * 게임 상태 변경 이벤트에 대한 델리게이트
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameStateChangedSignature, EPGGameStateType, NewState);
	
	/**
	 * 게임 상태 변경 이벤트
	 */
	UPROPERTY(BlueprintAssignable, Category = "Manager Subsystem")
	FOnGameStateChangedSignature OnGameStateChanged;

	/**
	 * 틱 함수 - FTSTicker에서 호출됩니다.
	 * @param DeltaTime 이전 프레임과의 시간 간격
	 * @return 계속 틱을 유지할지 여부
	 */
	bool TickInternal(float DeltaTime);

protected:
	/** 등록된 모든 매니저 목록 */
	UPROPERTY()
	TMap<FName, UObject*> RegisteredManagers;

	/** 현재 게임 상태 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Manager Subsystem")
	EPGGameStateType CurrentGameState;
	
	/** FTSTicker 틱 핸들 */
	FTSTicker::FDelegateHandle TickHandle;

	/**
	 * 매니저가 IPGManager 인터페이스를 구현하는지 확인
	 * @param Manager 확인할 매니저 인스턴스
	 * @return IPGManager 인터페이스 구현 여부
	 */
	bool ImplementsManagerInterface(UObject* Manager) const;
};

template<class T>
T* UPGManagerSubsystem::GetManager() const
{
	for (const auto& ManagerPair : RegisteredManagers)
	{
		T* TypedManager = Cast<T>(ManagerPair.Value);
		if (TypedManager)
		{
			return TypedManager;
		}
	}
	
	return nullptr;
}
