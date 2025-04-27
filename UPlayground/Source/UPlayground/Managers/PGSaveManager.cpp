// Copyright (c) 2025. All Rights Reserved.

#include "PGSaveManager.h"
#include "PGSaveGame.h"
#include "PGManagerSubsystem.h"
#include "PGInventoryManager.h"
#include "PGQuestManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "../Characters/PGPlayerCharacter.h"
#include "../Characters/PGBaseCharacter.h"

UPGSaveManager::UPGSaveManager()
{
	AutoSaveTimer = 0.0f;
	QuickSaveSlotName = TEXT("QuickSave");
	CurrentAutoSaveIndex = 0;
}

bool UPGSaveManager::OnInitialize_Implementation()
{
	if (!Super::OnInitialize_Implementation())
	{
		return false;
	}

	// 초기 메타데이터 캐시 업데이트
	UpdateMetaDataCache();

	return true;
}

void UPGSaveManager::OnShutdown_Implementation()
{
	Super::OnShutdown_Implementation();
	
	// 필요한 경우 마지막 자동 저장 수행
	if (AutoSaveConfig.bEnabled)
	{
		AutoSave();
	}
}

void UPGSaveManager::OnUpdate_Implementation(float DeltaTime)
{
	Super::OnUpdate_Implementation(DeltaTime);

	// 자동 저장 타이머 업데이트
	if (AutoSaveConfig.bEnabled)
	{
		AutoSaveTimer += DeltaTime;
		
		if (AutoSaveTimer >= AutoSaveConfig.Interval)
		{
			AutoSave();
			AutoSaveTimer = 0.0f;
		}
	}
}

void UPGSaveManager::OnGameStateChange_Implementation(EPGGameStateType NewState)
{
	Super::OnGameStateChange_Implementation(NewState);

	// 특정 게임 상태에서 자동 저장 수행
	if (NewState == EPGGameStateType::GamePaused && AutoSaveConfig.bEnabled)
	{
		AutoSave();
	}
}

bool UPGSaveManager::SaveGame(const FString& SlotName, EPGSaveSlotType SlotType, bool bOverwrite)
{
	// 슬롯 존재 여부 확인
	if (!bOverwrite && DoesSaveSlotExist(SlotName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save slot '%s' already exists and overwrite is disabled"), *SlotName);
		OnSaveCompleted.Broadcast(SlotName, false);
		return false;
	}

	// SaveGame 객체 생성
	UPGSaveGame* SaveGameInstance = CreateSaveGameObject(SlotName, SlotType);
	if (!SaveGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create SaveGame object"));
		OnSaveCompleted.Broadcast(SlotName, false);
		return false;
	}

	// 저장 데이터 수집
	if (!CollectSaveData(SaveGameInstance))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to collect save data"));
		OnSaveCompleted.Broadcast(SlotName, false);
		return false;
	}

	// 저장 수행
	if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0))
	{
		// 메타데이터 캐시 업데이트
		SaveSlotMetaDataCache.Add(SlotName, SaveGameInstance->MetaData);
		
		UE_LOG(LogTemp, Log, TEXT("Game saved successfully to slot: %s"), *SlotName);
		OnSaveCompleted.Broadcast(SlotName, true);
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot: %s"), *SlotName);
	OnSaveCompleted.Broadcast(SlotName, false);
	return false;
}

bool UPGSaveManager::LoadGame(const FString& SlotName)
{
	// 슬롯 존재 여부 확인
	if (!DoesSaveSlotExist(SlotName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save slot '%s' does not exist"), *SlotName);
		OnLoadCompleted.Broadcast(SlotName, false);
		return false;
	}

	// SaveGame 객체 로드
	UPGSaveGame* LoadedGame = Cast<UPGSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
	if (!LoadedGame)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load game from slot: %s"), *SlotName);
		OnLoadCompleted.Broadcast(SlotName, false);
		return false;
	}

	// 로드된 데이터 적용
	if (!ApplyLoadedData(LoadedGame))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to apply loaded data from slot: %s"), *SlotName);
		OnLoadCompleted.Broadcast(SlotName, false);
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Game loaded successfully from slot: %s"), *SlotName);
	OnLoadCompleted.Broadcast(SlotName, true);
	return true;
}

bool UPGSaveManager::DeleteSaveSlot(const FString& SlotName)
{
	if (UGameplayStatics::DeleteGameInSlot(SlotName, 0))
	{
		SaveSlotMetaDataCache.Remove(SlotName);
		
		UE_LOG(LogTemp, Log, TEXT("Save slot deleted: %s"), *SlotName);
		OnSaveDeleted.Broadcast(SlotName);
		return true;
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to delete save slot: %s"), *SlotName);
	return false;
}

bool UPGSaveManager::QuickSave()
{
	return SaveGame(QuickSaveSlotName, EPGSaveSlotType::Quick, true);
}

bool UPGSaveManager::QuickLoad()
{
	return LoadGame(QuickSaveSlotName);
}

bool UPGSaveManager::AutoSave()
{
	FString AutoSaveSlotName = GenerateSaveSlotName(EPGSaveSlotType::Auto);
	
	OnAutoSaveTriggered.Broadcast();
	
	bool bSuccess = SaveGame(AutoSaveSlotName, EPGSaveSlotType::Auto, true);
	
	if (bSuccess)
	{
		CleanupAutoSaveSlots();
	}
	
	return bSuccess;
}

TArray<FPGSaveMetaData> UPGSaveManager::GetAllSaveSlots()
{
	TArray<FPGSaveMetaData> SaveSlots;
	SaveSlotMetaDataCache.GenerateValueArray(SaveSlots);
	
	// 저장 시간 기준으로 정렬 (최신순)
	SaveSlots.Sort([](const FPGSaveMetaData& A, const FPGSaveMetaData& B)
	{
		return A.SaveDateTime > B.SaveDateTime;
	});
	
	return SaveSlots;
}

bool UPGSaveManager::DoesSaveSlotExist(const FString& SlotName) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

bool UPGSaveManager::GetSaveSlotMetaData(const FString& SlotName, FPGSaveMetaData& OutMetaData) const
{
	if (const FPGSaveMetaData* MetaData = SaveSlotMetaDataCache.Find(SlotName))
	{
		OutMetaData = *MetaData;
		return true;
	}
	
	return false;
}

void UPGSaveManager::SetAutoSaveConfig(const FPGAutoSaveConfig& NewConfig)
{
	AutoSaveConfig = NewConfig;
	
	// 자동 저장이 비활성화되면 타이머 초기화
	if (!AutoSaveConfig.bEnabled)
	{
		AutoSaveTimer = 0.0f;
	}
}

UPGSaveGame* UPGSaveManager::CreateSaveGameObject(const FString& SlotName, EPGSaveSlotType SlotType)
{
	UPGSaveGame* SaveGameInstance = Cast<UPGSaveGame>(UGameplayStatics::CreateSaveGameObject(UPGSaveGame::StaticClass()));
	if (!SaveGameInstance)
	{
		return nullptr;
	}

	// 메타데이터 설정
	SaveGameInstance->MetaData.SlotName = SlotName;
	SaveGameInstance->MetaData.SlotType = SlotType;
	
	return SaveGameInstance;
}

bool UPGSaveManager::CollectSaveData(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	// 플레이어 데이터 수집
	if (!CollectPlayerData(SaveGame))
	{
		return false;
	}

	// 인벤토리 데이터 수집
	if (!CollectInventoryData(SaveGame))
	{
		return false;
	}

	// 퀘스트 데이터 수집
	if (!CollectQuestData(SaveGame))
	{
		return false;
	}

	// 메타데이터 업데이트
	UWorld* World = GetWorld();
	if (World)
	{
		SaveGame->UpdateMetaData(
			TEXT("Player"), // 실제 캐릭터 이름 필요
			SaveGame->PlayerLevel,
			World->GetTimeSeconds(),
			World->GetMapName()
		);
	}

	return true;
}

bool UPGSaveManager::ApplyLoadedData(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	// 플레이어 데이터 적용
	if (!ApplyPlayerData(SaveGame))
	{
		return false;
	}

	// 인벤토리 데이터 적용
	if (!ApplyInventoryData(SaveGame))
	{
		return false;
	}

	// 퀘스트 데이터 적용
	if (!ApplyQuestData(SaveGame))
	{
		return false;
	}

	return true;
}

void UPGSaveManager::UpdateMetaDataCache()
{
	SaveSlotMetaDataCache.Empty();
	
	// 모든 저장 슬롯의 메타데이터 로드
	// 언리얼 엔진의 기본 save game 시스템을 사용하여 슬롯 이름을 수동으로 추적
	// 실제 프로젝트에서는 저장된 슬롯 목록을 별도로 관리해야 함
	
	// 예시: 알려진 슬롯 이름을 수동으로 확인
	TArray<FString> KnownSlotNames = {
		QuickSaveSlotName,
		TEXT("AutoSave_0"),
		TEXT("AutoSave_1"),
		TEXT("AutoSave_2")
		// 추가 슬롯들...
	};
	
	for (const FString& SlotName : KnownSlotNames)
	{
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			if (UPGSaveGame* SaveGame = Cast<UPGSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0)))
			{
				SaveSlotMetaDataCache.Add(SlotName, SaveGame->MetaData);
			}
		}
	}
}

void UPGSaveManager::CleanupAutoSaveSlots()
{
	TArray<FPGSaveMetaData> AutoSaves;
	
	// 자동 저장 슬롯만 필터링
	for (const auto& Pair : SaveSlotMetaDataCache)
	{
		if (Pair.Value.SlotType == EPGSaveSlotType::Auto)
		{
			AutoSaves.Add(Pair.Value);
		}
	}
	
	// 저장 시간 기준 정렬 (오래된 순)
	AutoSaves.Sort([](const FPGSaveMetaData& A, const FPGSaveMetaData& B)
	{
		return A.SaveDateTime < B.SaveDateTime;
	});
	
	// 최대 자동 저장 수 초과 시 오래된 슬롯 삭제
	while (AutoSaves.Num() > AutoSaveConfig.MaxAutoSaves)
	{
		DeleteSaveSlot(AutoSaves[0].SlotName);
		AutoSaves.RemoveAt(0);
	}
}

FString UPGSaveManager::GenerateSaveSlotName(EPGSaveSlotType SlotType)
{
	FString SlotName;
	
	switch (SlotType)
	{
	case EPGSaveSlotType::Auto:
		CurrentAutoSaveIndex = (CurrentAutoSaveIndex + 1) % AutoSaveConfig.MaxAutoSaves;
		SlotName = FString::Printf(TEXT("AutoSave_%d"), CurrentAutoSaveIndex);
		break;
		
	case EPGSaveSlotType::Quick:
		SlotName = QuickSaveSlotName;
		break;
		
	case EPGSaveSlotType::Checkpoint:
		SlotName = FString::Printf(TEXT("Checkpoint_%s"), *FDateTime::Now().ToString());
		break;
		
	default:
		SlotName = FString::Printf(TEXT("Manual_%s"), *FDateTime::Now().ToString());
		break;
	}
	
	return SlotName;
}

bool UPGSaveManager::CollectPlayerData(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		return false;
	}

	APGBaseCharacter* PlayerCharacter = Cast<APGBaseCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		return false;
	}

	// 플레이어 위치 및 회전 저장
	SaveGame->PlayerLocation = PlayerCharacter->GetActorLocation();
	SaveGame->PlayerRotation = PlayerCharacter->GetActorRotation();
	
	// 플레이어 스탯 저장
	SaveGame->PlayerHealth = PlayerCharacter->GetHealth();
	SaveGame->PlayerMaxHealth = PlayerCharacter->GetMaxHealth();
	
	// 레벨 및 경험치 (예시 - 실제 구현에 맞게 수정 필요)
	SaveGame->PlayerLevel = 1;  // 실제 레벨 시스템 구현 필요
	SaveGame->PlayerExperience = 0;  // 실제 경험치 시스템 구현 필요

	return true;
}

bool UPGSaveManager::ApplyPlayerData(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		return false;
	}

	APGBaseCharacter* PlayerCharacter = Cast<APGBaseCharacter>(PlayerController->GetPawn());
	if (!PlayerCharacter)
	{
		return false;
	}

	// 플레이어 위치 및 회전 적용
	PlayerCharacter->SetActorLocation(SaveGame->PlayerLocation);
	PlayerCharacter->SetActorRotation(SaveGame->PlayerRotation);
	
	// 플레이어 스탯 적용
	PlayerCharacter->SetHealth(SaveGame->PlayerHealth);
	
	// 레벨 및 경험치 적용 (예시 - 실제 구현에 맞게 수정 필요)
	// PlayerCharacter->SetLevel(SaveGame->PlayerLevel);
	// PlayerCharacter->SetExperience(SaveGame->PlayerExperience);

	return true;
}

bool UPGSaveManager::CollectInventoryData(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return false;
	}

	UPGManagerSubsystem* ManagerSubsystem = GameInstance->GetSubsystem<UPGManagerSubsystem>();
	if (!ManagerSubsystem)
	{
		return false;
	}

	UPGInventoryManager* InventoryManager = ManagerSubsystem->GetManager<UPGInventoryManager>();
	if (!InventoryManager)
	{
		return false;
	}

	// IPGSavableInterface 구현 필요
	if (InventoryManager->GetClass()->ImplementsInterface(UPGSavableInterface::StaticClass()))
	{
		IPGSavableInterface::Execute_SaveData(InventoryManager, SaveGame);
	}

	return true;
}

bool UPGSaveManager::ApplyInventoryData(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return false;
	}

	UPGManagerSubsystem* ManagerSubsystem = GameInstance->GetSubsystem<UPGManagerSubsystem>();
	if (!ManagerSubsystem)
	{
		return false;
	}

	UPGInventoryManager* InventoryManager = ManagerSubsystem->GetManager<UPGInventoryManager>();
	if (!InventoryManager)
	{
		return false;
	}

	// IPGSavableInterface 구현 필요
	if (InventoryManager->GetClass()->ImplementsInterface(UPGSavableInterface::StaticClass()))
	{
		IPGSavableInterface::Execute_LoadData(InventoryManager, SaveGame);
	}

	return true;
}

bool UPGSaveManager::CollectQuestData(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return false;
	}

	UPGManagerSubsystem* ManagerSubsystem = GameInstance->GetSubsystem<UPGManagerSubsystem>();
	if (!ManagerSubsystem)
	{
		return false;
	}

	UPGQuestManager* QuestManager = ManagerSubsystem->GetManager<UPGQuestManager>();
	if (!QuestManager)
	{
		return false;
	}

	// IPGSavableInterface 구현 필요
	if (QuestManager->GetClass()->ImplementsInterface(UPGSavableInterface::StaticClass()))
	{
		IPGSavableInterface::Execute_SaveData(QuestManager, SaveGame);
	}

	return true;
}

bool UPGSaveManager::ApplyQuestData(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return false;
	}

	UPGManagerSubsystem* ManagerSubsystem = GameInstance->GetSubsystem<UPGManagerSubsystem>();
	if (!ManagerSubsystem)
	{
		return false;
	}

	UPGQuestManager* QuestManager = ManagerSubsystem->GetManager<UPGQuestManager>();
	if (!QuestManager)
	{
		return false;
	}

	// IPGSavableInterface 구현 필요
	if (QuestManager->GetClass()->ImplementsInterface(UPGSavableInterface::StaticClass()))
	{
		IPGSavableInterface::Execute_LoadData(QuestManager, SaveGame);
	}

	return true;
}
