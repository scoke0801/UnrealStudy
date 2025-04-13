// Copyright (c) 2025. All Rights Reserved.

#include "PGBaseManager.h"

#include "PGGameStateTypes.h"

UPGBaseManager::UPGBaseManager()
	: bIsActive(false)
	, ManagerName(NAME_None)
	, CurrentGameState(EPGGameStateType::None)
{
	// 기본 생성자
}

bool UPGBaseManager::Initialize()
{
	// 자식 클래스의 초기화 로직 호출
	bool bSuccess = OnInitialize();
	if (bSuccess)
	{
		bIsActive = true;
		UE_LOG(LogTemp, Log, TEXT("매니저 초기화: %s"), *GetManagerName().ToString());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("매니저 초기화 실패: %s"), *GetManagerName().ToString());
	}

	return bSuccess;
}

void UPGBaseManager::Shutdown()
{
	if (bIsActive)
	{
		// 자식 클래스의 종료 로직 호출
		OnShutdown();
		bIsActive = false;
		UE_LOG(LogTemp, Log, TEXT("매니저 종료: %s"), *GetManagerName().ToString());
	}
}

bool UPGBaseManager::IsActive() const
{
	return bIsActive;
}

FName UPGBaseManager::GetManagerName() const
{
	return ManagerName;
}

void UPGBaseManager::Update(float DeltaTime)
{
	if (bIsActive)
	{
		// 자식 클래스의 업데이트 로직 호출
		OnUpdate(DeltaTime);
	}
}

void UPGBaseManager::OnGameStateChanged(EPGGameStateType NewState)
{
	CurrentGameState = NewState;

	// 새 상태에서 매니저의 활성화 여부 결정
	bool bShouldBeActive = ShouldBeActiveInState(NewState);
	
	if (bShouldBeActive != bIsActive)
	{
		bIsActive = bShouldBeActive;
		if (bIsActive)
		{
			UE_LOG(LogTemp, Log, TEXT("매니저 활성화: %s (상태: %d)"), *GetManagerName().ToString(), static_cast<int32>(NewState));
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("매니저 비활성화: %s (상태: %d)"), *GetManagerName().ToString(), static_cast<int32>(NewState));
		}
	}

	// 자식 클래스의 상태 변경 로직 호출
	OnGameStateChange(NewState);
}

bool UPGBaseManager::ShouldBeActiveInState(EPGGameStateType GameState) const
{
	// ReactToGameStates 배열이 비어있으면 항상 활성화
	if (ReactToGameStates.Num() == 0)
	{
		return true;
	}

	// 현재 게임 상태가 반응해야 하는 상태 목록에 있는지 확인
	return ReactToGameStates.Contains(GameState);
}

bool UPGBaseManager::OnInitialize_Implementation()
{
	// 기본 구현은 성공을 반환
	return true;
}

void UPGBaseManager::OnShutdown_Implementation()
{
	// 기본 구현은 비어있음
}

void UPGBaseManager::OnUpdate_Implementation(float DeltaTime)
{
	// 기본 구현은 비어있음
}

void UPGBaseManager::OnGameStateChange_Implementation(EPGGameStateType NewState)
{
	// 기본 구현은 비어있음
}
