// Copyright (c) 2025. All Rights Reserved.

#include "PGManagerSubsystem.h"
#include "PGIManager.h"
#include "Engine/Engine.h"
#include "Misc/ScopeLock.h"
#include "TimerManager.h"

UPGManagerSubsystem::UPGManagerSubsystem()
	: CurrentGameState(EPGGameStateType::None)
{
	// 기본 생성자
}

void UPGManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogTemp, Log, TEXT("매니저 서브시스템 초기화"));

	// 매니저 서브시스템이 Tick을 받도록 설정
	// UE5.4에서는 FTSTicker를 사용하여 틱 이벤트를 처리합니다
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateUObject(this, &UPGManagerSubsystem::TickInternal),
		0.0f);
}

void UPGManagerSubsystem::Deinitialize()
{
	// Tick 이벤트 구독 취소
	if (TickHandle.IsValid())
	{
		FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
		TickHandle.Reset();
	}

	// 모든 등록된 매니저 종료
	TArray<FName> ManagerNames;
	RegisteredManagers.GetKeys(ManagerNames);
	
	for (const FName& ManagerName : ManagerNames)
	{
		UnregisterManager(ManagerName);
	}
	
	RegisteredManagers.Empty();

	UE_LOG(LogTemp, Log, TEXT("매니저 서브시스템 종료"));

	Super::Deinitialize();
}

bool UPGManagerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// GameInstance에서만 이 서브시스템을 생성하도록 함
	return Outer->IsA<UGameInstance>();
}

bool UPGManagerSubsystem::RegisterManager(UObject* Manager)
{
	if (!Manager)
	{
		UE_LOG(LogTemp, Error, TEXT("매니저 등록 실패: 유효하지 않은 매니저 인스턴스"));
		return false;
	}

	if (!ImplementsManagerInterface(Manager))
	{
		UE_LOG(LogTemp, Error, TEXT("매니저 등록 실패: IPGManager 인터페이스를 구현하지 않음 - %s"), *Manager->GetClass()->GetName());
		return false;
	}

	IPGManager* ManagerInterface = Cast<IPGManager>(Manager);
	FName ManagerName = ManagerInterface->GetManagerName();

	if (ManagerName == NAME_None)
	{
		UE_LOG(LogTemp, Error, TEXT("매니저 등록 실패: 유효하지 않은 매니저 이름"));
		return false;
	}

	if (RegisteredManagers.Contains(ManagerName))
	{
		UE_LOG(LogTemp, Warning, TEXT("매니저 등록 실패: 이미 등록된 매니저 이름 - %s"), *ManagerName.ToString());
		return false;
	}

	// 매니저 초기화
	if (!ManagerInterface->Initialize())
	{
		UE_LOG(LogTemp, Error, TEXT("매니저 등록 실패: 초기화 실패 - %s"), *ManagerName.ToString());
		return false;
	}

	// 매니저 등록
	RegisteredManagers.Add(ManagerName, Manager);
	
	// 현재 게임 상태 알림
	ManagerInterface->OnGameStateChanged(CurrentGameState);

	UE_LOG(LogTemp, Log, TEXT("매니저 등록 성공: %s"), *ManagerName.ToString());
	return true;
}

bool UPGManagerSubsystem::UnregisterManager(FName ManagerName)
{
	UObject** FoundManager = RegisteredManagers.Find(ManagerName);
	if (!FoundManager || !(*FoundManager))
	{
		UE_LOG(LogTemp, Warning, TEXT("매니저 해제 실패: 등록되지 않은 매니저 - %s"), *ManagerName.ToString());
		return false;
	}

	IPGManager* ManagerInterface = Cast<IPGManager>(*FoundManager);
	if (ManagerInterface)
	{
		// 매니저 종료
		ManagerInterface->Shutdown();
	}

	// 매니저 등록 해제
	RegisteredManagers.Remove(ManagerName);

	UE_LOG(LogTemp, Log, TEXT("매니저 해제 성공: %s"), *ManagerName.ToString());
	return true;
}

UObject* UPGManagerSubsystem::GetManagerByName(FName ManagerName) const
{
	UObject* const* FoundManager = RegisteredManagers.Find(ManagerName);
	return FoundManager ? *FoundManager : nullptr;
}

void UPGManagerSubsystem::SetGameState(EPGGameStateType NewState)
{
	if (CurrentGameState == NewState)
	{
		return;
	}

	CurrentGameState = NewState;
	UE_LOG(LogTemp, Log, TEXT("게임 상태 변경: %d"), static_cast<int32>(NewState));

	// 모든 매니저에 상태 변경 알림
	for (const auto& ManagerPair : RegisteredManagers)
	{
		IPGManager* ManagerInterface = Cast<IPGManager>(ManagerPair.Value);
		if (ManagerInterface)
		{
			ManagerInterface->OnGameStateChanged(NewState);
		}
	}

	// 델리게이트 브로드캐스트
	OnGameStateChanged.Broadcast(NewState);
}

EPGGameStateType UPGManagerSubsystem::GetCurrentGameState() const
{
	return CurrentGameState;
}

bool UPGManagerSubsystem::TickInternal(float DeltaTime)
{
	// 모든 활성화된 매니저 업데이트
	for (const auto& ManagerPair : RegisteredManagers)
	{
		IPGManager* ManagerInterface = Cast<IPGManager>(ManagerPair.Value);
		if (ManagerInterface && ManagerInterface->IsActive())
		{
			ManagerInterface->Update(DeltaTime);
		}
	}
	
	// 계속 틱을 유지하기 위해 true 반환
	return true;
}

bool UPGManagerSubsystem::ImplementsManagerInterface(UObject* Manager) const
{
	return Manager && Manager->GetClass()->ImplementsInterface(UPGManager::StaticClass());
}
