// Copyright (c) 2025. All Rights Reserved.

#include "PGGameFeatureManagerAction.h"
#include "PGManagerSubsystem.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"

UPGGameFeatureManagerAction::UPGGameFeatureManagerAction()
{
	// 기본 생성자
}

UPGGameFeatureManagerAction::~UPGGameFeatureManagerAction()
{
	// 모든 매니저 정리 (안전을 위해)
	RemoveManagersFromAllWorlds();
}

void UPGGameFeatureManagerAction::OnGameFeatureRegistering()
{
	Super::OnGameFeatureRegistering();
	// 등록 시 필요한 작업이 있다면 여기에 구현
}

void UPGGameFeatureManagerAction::OnGameFeatureUnregistering()
{
	// 등록 해제 시 매니저 제거
	RemoveManagersFromAllWorlds();
	Super::OnGameFeatureUnregistering();
}

void UPGGameFeatureManagerAction::OnGameFeatureActivating()
{
	// 게임 피처 활성화 시 매니저 추가
	AddManagersToAllWorlds();
	Super::OnGameFeatureActivating();
}

void UPGGameFeatureManagerAction::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	// 게임 피처 비활성화 시 매니저 제거
	RemoveManagersFromAllWorlds();
}

void UPGGameFeatureManagerAction::AddManagersToAllWorlds()
{
	// 모든 게임 인스턴스에 매니저 추가
	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		UWorld* World = WorldContext.World();
		UGameInstance* GameInstance = WorldContext.OwningGameInstance;
		
		if (GameInstance && World && World->IsGameWorld())
		{
			AddManagersToGameInstance(GameInstance);
		}
	}
}

void UPGGameFeatureManagerAction::RemoveManagersFromAllWorlds()
{
	// 모든 등록된 게임 인스턴스에서 매니저 제거
	TArray<TWeakObjectPtr<UGameInstance>> GameInstancesToRemove;
	for (auto& Pair : SpawnedManagers)
	{
		if (UGameInstance* GameInstance = Pair.Key.Get())
		{
			GameInstancesToRemove.Add(Pair.Key);
		}
	}

	for (const TWeakObjectPtr<UGameInstance>& GameInstancePtr : GameInstancesToRemove)
	{
		if (UGameInstance* GameInstance = GameInstancePtr.Get())
		{
			RemoveManagersFromGameInstance(GameInstance);
		}
	}

	SpawnedManagers.Empty();
}

void UPGGameFeatureManagerAction::AddManagersToGameInstance(UGameInstance* GameInstance)
{
	if (!GameInstance)
	{
		return;
	}

	// 이미 이 게임 인스턴스에 매니저가 등록되어 있는지 확인
	TWeakObjectPtr<UGameInstance> GameInstancePtr(GameInstance);
	if (SpawnedManagers.Contains(GameInstancePtr))
	{
		return;
	}

	// 게임 인스턴스에 매니저 등록
	TArray<TWeakObjectPtr<UObject>> NewManagers = RegisterManagersForGameInstance(GameInstance);
	if (NewManagers.Num() > 0)
	{
		SpawnedManagers.Add(GameInstancePtr, MoveTemp(NewManagers));
	}
}

void UPGGameFeatureManagerAction::RemoveManagersFromGameInstance(UGameInstance* GameInstance)
{
	if (!GameInstance)
	{
		return;
	}

	// 게임 인스턴스에서 등록한 매니저 제거
	TWeakObjectPtr<UGameInstance> GameInstancePtr(GameInstance);
	TArray<TWeakObjectPtr<UObject>>* ManagerArray = SpawnedManagers.Find(GameInstancePtr);
	if (!ManagerArray)
	{
		return;
	}

	UPGManagerSubsystem* ManagerSubsystem = GameInstance->GetSubsystem<UPGManagerSubsystem>();
	if (ManagerSubsystem)
	{
		// 모든 매니저 해제
		for (const TWeakObjectPtr<UObject>& ManagerPtr : *ManagerArray)
		{
			if (UObject* Manager = ManagerPtr.Get())
			{
				IPGManager* ManagerInterface = Cast<IPGManager>(Manager);
				if (ManagerInterface)
				{
					ManagerSubsystem->UnregisterManager(ManagerInterface->GetManagerName());
				}
			}
		}
	}

	SpawnedManagers.Remove(GameInstancePtr);
}

TArray<TWeakObjectPtr<UObject>> UPGGameFeatureManagerAction::RegisterManagersForGameInstance(UGameInstance* GameInstance)
{
	TArray<TWeakObjectPtr<UObject>> NewManagers;
	if (!GameInstance)
	{
		return NewManagers;
	}

	UPGManagerSubsystem* ManagerSubsystem = GameInstance->GetSubsystem<UPGManagerSubsystem>();
	if (!ManagerSubsystem)
	{
		return NewManagers;
	}

	// 모든 매니저 클래스 생성 및 등록
	for (const TSubclassOf<UObject>& ManagerClass : ManagerClasses)
	{
		if (ManagerClass)
		{
			UObject* NewManager = NewObject<UObject>(GameInstance, ManagerClass);
			if (NewManager && ManagerSubsystem->RegisterManager(NewManager))
			{
				NewManagers.Add(NewManager);
			}
		}
	}

	return NewManagers;
}
