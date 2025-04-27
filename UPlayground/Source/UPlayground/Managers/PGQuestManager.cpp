// Copyright (c) 2025. All Rights Reserved.

#include "PGQuestManager.h"
#include "PGInventoryManager.h"
#include "PGManagerSubsystem.h"
#include "PGSaveGame.h"
#include "GameFramework/SaveGame.h"
#include "Kismet/GameplayStatics.h"

UPGQuestManager::UPGQuestManager()
	: SaveTimer(0.0f)
	, AutoSaveInterval(300.0f) // 5분
{
	ManagerName = FName("QuestManager");
	
	// 퀘스트 매니저는 인게임 상태에서 활성화
	ReactToGameStates = {
		EPGGameStateType::GameRunning,
		EPGGameStateType::Dialog
	};
}

bool UPGQuestManager::OnInitialize_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 초기화 중..."));
	
	// 퀘스트 데이터 로드
	if (!LoadQuestData())
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 데이터 로드 실패, 기본 퀘스트 데이터 사용"));
	}
	
	return true;
}

void UPGQuestManager::OnShutdown_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 종료 중..."));
	
	// 퀘스트 데이터 저장
	if (!SaveQuestData())
	{
		UE_LOG(LogTemp, Error, TEXT("[QuestManager] 퀘스트 데이터 저장 실패"));
	}
}

void UPGQuestManager::OnUpdate_Implementation(float DeltaTime)
{
	// 자동 저장 타이머 업데이트
	if (AutoSaveInterval > 0.0f)
	{
		SaveTimer += DeltaTime;
		if (SaveTimer >= AutoSaveInterval)
		{
			SaveTimer = 0.0f;
			SaveQuestData();
		}
	}
	
	// 활성 퀘스트의 타이머 업데이트
	for (const FName& QuestID : ActiveQuests)
	{
		FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
		if (QuestInfo && QuestInfo->QuestData.TimeLimit > 0.0f)
		{
			// 제한 시간이 다 되면 퀘스트 실패
			if (QuestInfo->IsExpired())
			{
				FailQuest(QuestID);
			}
		}
	}
}

void UPGQuestManager::OnGameStateChange_Implementation(EPGGameStateType NewState)
{
	// 게임 상태 변경에 따른 처리
	if (NewState == EPGGameStateType::Dialog)
	{
		UE_LOG(LogTemp, Log, TEXT("[QuestManager] 대화 모드 진입"));
	}
}

bool UPGQuestManager::StartQuest(FName QuestID)
{
	if (QuestID == NAME_None)
	{
		return false;
	}
	
	// 퀘스트 정보 확인
	FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	if (!QuestInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 시작 실패: 존재하지 않는 퀘스트 ID - %s"), *QuestID.ToString());
		return false;
	}
	
	// 이미 활성화된 퀘스트인지 확인
	if (QuestInfo->State == EPGQuestState::Active)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 시작 실패: 이미 활성화된 퀘스트 - %s"), *QuestID.ToString());
		return false;
	}
	
	// 이미 완료된 퀘스트인지 확인
	if (QuestInfo->State == EPGQuestState::Completed && !QuestInfo->QuestData.bIsRepeatable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 시작 실패: 이미 완료된 퀘스트 (반복 불가) - %s"), *QuestID.ToString());
		return false;
	}
	
	// 선행 퀘스트 완료 여부 확인
	for (const FName& PrerequisiteQuestID : QuestInfo->QuestData.PrerequisiteQuests)
	{
		if (GetQuestStatus(PrerequisiteQuestID) != EPGQuestState::Completed)
		{
			UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 시작 실패: 선행 퀘스트 미완료 - %s -> %s"), 
				*PrerequisiteQuestID.ToString(), *QuestID.ToString());
			return false;
		}
	}
	
	// 퀘스트 상태 변경
	if (!SetQuestStatus(QuestID, EPGQuestState::Active))
	{
		return false;
	}
	
	// 활성 퀘스트 목록에 추가
	if (!ActiveQuests.Contains(QuestID))
	{
		ActiveQuests.Add(QuestID);
	}
	
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 시작: %s"), *QuestID.ToString());
	return true;
}

bool UPGQuestManager::CompleteQuest(FName QuestID)
{
	if (QuestID == NAME_None)
	{
		return false;
	}
	
	// 퀘스트 정보 확인
	FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	if (!QuestInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 완료 실패: 존재하지 않는 퀘스트 ID - %s"), *QuestID.ToString());
		return false;
	}
	
	// 활성화된 퀘스트인지 확인
	if (QuestInfo->State != EPGQuestState::Active)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 완료 실패: 활성화되지 않은 퀘스트 - %s"), *QuestID.ToString());
		return false;
	}
	
	// 모든 목표가 완료되었는지 확인
	if (!QuestInfo->AreAllObjectivesCompleted())
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 완료 실패: 미완료된 필수 목표 존재 - %s"), *QuestID.ToString());
		return false;
	}
	
	// 퀘스트 상태 변경
	if (!SetQuestStatus(QuestID, EPGQuestState::Completed))
	{
		return false;
	}
	
	// 활성 퀘스트 목록에서 제거
	ActiveQuests.Remove(QuestID);
	
	// 완료된 퀘스트 목록에 추가
	if (!CompletedQuests.Contains(QuestID))
	{
		CompletedQuests.Add(QuestID);
	}
	
	// 퀘스트 보상 제공
	GiveQuestRewards(QuestID);
	
	// 퀘스트 완료 이벤트 발생
	OnQuestCompleted.Broadcast(QuestID);
	
	// 후속 퀘스트 활성화
	for (const FName& FollowupQuestID : QuestInfo->QuestData.NextQuests)
	{
		FPGActiveQuest* FollowupQuestInfo = QuestData.Find(FollowupQuestID);
		if (FollowupQuestInfo && FollowupQuestInfo->State == EPGQuestState::Inactive)
		{
			FollowupQuestInfo->State = EPGQuestState::Active;
			UE_LOG(LogTemp, Log, TEXT("[QuestManager] 후속 퀘스트 활성화: %s"), *FollowupQuestID.ToString());
			
			// 상태 변경 이벤트 브로드캐스트
			OnQuestStatusChanged.Broadcast(FollowupQuestID);
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 완료: %s"), *QuestID.ToString());
	return true;
}

bool UPGQuestManager::FailQuest(FName QuestID)
{
	if (QuestID == NAME_None)
	{
		return false;
	}
	
	// 퀘스트 정보 확인
	FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	if (!QuestInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 실패 처리 실패: 존재하지 않는 퀘스트 ID - %s"), *QuestID.ToString());
		return false;
	}
	
	// 활성화된 퀘스트인지 확인
	if (QuestInfo->State != EPGQuestState::Active)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 실패 처리 실패: 활성화되지 않은 퀘스트 - %s"), *QuestID.ToString());
		return false;
	}
	
	// 퀘스트 상태 변경
	if (!SetQuestStatus(QuestID, EPGQuestState::Failed))
	{
		return false;
	}
	
	// 활성 퀘스트 목록에서 제거
	ActiveQuests.Remove(QuestID);
	
	// 실패한 퀘스트 목록에 추가
	if (!FailedQuests.Contains(QuestID))
	{
		FailedQuests.Add(QuestID);
	}
	
	// 퀘스트 실패 이벤트 발생
	OnQuestFailed.Broadcast(QuestID);
	
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 실패: %s"), *QuestID.ToString());
	return true;
}

bool UPGQuestManager::AbandonQuest(FName QuestID)
{
	if (QuestID == NAME_None)
	{
		return false;
	}
	
	// 퀘스트 정보 확인
	FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	if (!QuestInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 포기 실패: 존재하지 않는 퀘스트 ID - %s"), *QuestID.ToString());
		return false;
	}
	
	// 활성화된 퀘스트인지 확인
	if (QuestInfo->State != EPGQuestState::Active)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 포기 실패: 활성화되지 않은 퀘스트 - %s"), *QuestID.ToString());
		return false;
	}
	
	// 퀘스트 상태 변경
	if (!SetQuestStatus(QuestID, EPGQuestState::Abandoned))
	{
		return false;
	}
	
	// 활성 퀘스트 목록에서 제거
	ActiveQuests.Remove(QuestID);
	
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 포기: %s"), *QuestID.ToString());
	return true;
}

bool UPGQuestManager::UpdateQuestObjective(FName QuestID, FName ObjectiveID, int32 Progress)
{
	if (QuestID == NAME_None || ObjectiveID == NAME_None)
	{
		return false;
	}
	
	// 퀘스트 정보 확인
	FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	if (!QuestInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 목표 업데이트 실패: 존재하지 않는 퀘스트 ID - %s"), *QuestID.ToString());
		return false;
	}
	
	// 활성화된 퀘스트인지 확인
	if (QuestInfo->State != EPGQuestState::Active)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 목표 업데이트 실패: 활성화되지 않은 퀘스트 - %s"), *QuestID.ToString());
		return false;
	}
	
	// 목표 찾기 및 진행도 업데이트
	bool bObjectiveCompleted = QuestInfo->ProgressObjective(ObjectiveID, Progress);
	
	// 목표 업데이트 이벤트 발생
	OnQuestObjectiveUpdated.Broadcast(QuestID, ObjectiveID);
	
	// 모든 목표가 완료되었는지 확인하고, 자동 완료 처리
	if (QuestInfo->AreAllObjectivesCompleted())
	{
		UE_LOG(LogTemp, Log, TEXT("[QuestManager] 모든 퀘스트 목표 완료: %s"), *QuestID.ToString());
		
		// 자동 완료 처리가 필요한 경우 여기에 구현
		// 예: 특정 퀘스트 타입에 대한 자동 완료
		if (QuestInfo->QuestData.CompleteNPCID == NAME_None)
		{
			CompleteQuest(QuestID);
		}
	}
	
	return true;
}

EPGQuestState UPGQuestManager::GetQuestStatus(FName QuestID) const
{
	if (QuestID == NAME_None)
	{
		return EPGQuestState::Inactive;
	}
	
	const FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	return QuestInfo ? QuestInfo->State : EPGQuestState::Inactive;
}

bool UPGQuestManager::GetQuestInfo(FName QuestID, FPGActiveQuest& OutQuestInfo) const
{
	if (QuestID == NAME_None)
	{
		return false;
	}
	
	const FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	if (!QuestInfo)
	{
		return false;
	}
	
	OutQuestInfo = *QuestInfo;
	return true;
}

TArray<FName> UPGQuestManager::GetActiveQuests() const
{
	return ActiveQuests;
}

TArray<FName> UPGQuestManager::GetCompletedQuests() const
{
	return CompletedQuests;
}

TArray<FName> UPGQuestManager::GetFailedQuests() const
{
	return FailedQuests;
}

bool UPGQuestManager::SaveQuestData()
{
	// 저장 시스템 예시 (실제 구현은 게임의 저장 시스템에 따라 달라질 수 있음)
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 데이터 저장 중..."));
	
	// 실제 구현 시에는 USaveGame 클래스를 활용한 저장 시스템 구현
	return true;
}

bool UPGQuestManager::LoadQuestData()
{
	// 로드 시스템 예시 (실제 구현은 게임의 저장 시스템에 따라 달라질 수 있음)
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 데이터 로드 중..."));
	
	// 실제 구현 시에는 USaveGame 클래스를 활용한 로드 시스템 구현
	
	// 테스트용 초기 퀘스트 데이터 설정
	// 메인 퀘스트
	FPGQuestData MainQuestData;
	MainQuestData.QuestID = FName("MainQuest_001");
	MainQuestData.Title = FText::FromString("마을의 위기");
	MainQuestData.Description = FText::FromString("마을을 위협하는 적들을 물리치고 주민들을 보호하세요.");
	MainQuestData.Type = EPGQuestType::Main;
	MainQuestData.Level = 1;
	MainQuestData.StartNPCID = FName("NPC_Village_Chief");
	
	// 퀘스트 목표 추가
	FPGQuestObjective Objective1;
	Objective1.ObjectiveID = FName("MainQuest_001_Obj1");
	Objective1.Description = FText::FromString("마을 주변의 적 10마리 처치");
	Objective1.Type = EPGQuestObjectiveType::Kill;
	Objective1.TargetID = FName("Enemy_Goblin");
	Objective1.RequiredCount = 10;
	MainQuestData.Objectives.Add(Objective1);
	
	FPGQuestObjective Objective2;
	Objective2.ObjectiveID = FName("MainQuest_001_Obj2");
	Objective2.Description = FText::FromString("마을 주민들과 대화");
	Objective2.Type = EPGQuestObjectiveType::Talk;
	Objective2.TargetID = FName("NPC_Villager");
	Objective2.RequiredCount = 3;
	MainQuestData.Objectives.Add(Objective2);
	
	// 퀘스트 보상 설정
	FPGQuestReward Reward1;
	Reward1.Type = EPGQuestRewardType::Experience;
	Reward1.RewardID = NAME_None;
	Reward1.Amount = 100;
	Reward1.Description = FText::FromString("경험치 100 획득");
	MainQuestData.Rewards.Add(Reward1);
	
	FPGQuestReward Reward2;
	Reward2.Type = EPGQuestRewardType::Currency;
	Reward2.RewardID = NAME_None;
	Reward2.Amount = 50;
	Reward2.Description = FText::FromString("골드 50 획득");
	MainQuestData.Rewards.Add(Reward2);
	
	FPGQuestReward Reward3;
	Reward3.Type = EPGQuestRewardType::Item;
	Reward3.RewardID = FName("Item_Healing_Potion");
	Reward3.Amount = 3;
	Reward3.Description = FText::FromString("체력 물약 3개 획득");
	MainQuestData.Rewards.Add(Reward3);
	
	// 활성 퀘스트 생성 및 추가
	FPGActiveQuest MainQuest(MainQuestData);
	QuestData.Add(MainQuestData.QuestID, MainQuest);
	
	// 사이드 퀘스트
	FPGQuestData SideQuestData;
	SideQuestData.QuestID = FName("SideQuest_001");
	SideQuestData.Title = FText::FromString("약초 수집");
	SideQuestData.Description = FText::FromString("마을 의사를 위해 희귀 약초를 수집하세요.");
	SideQuestData.Type = EPGQuestType::Side;
	SideQuestData.Level = 2;
	SideQuestData.StartNPCID = FName("NPC_Village_Doctor");
	
	// 퀘스트 목표 추가
	FPGQuestObjective SideObjective1;
	SideObjective1.ObjectiveID = FName("SideQuest_001_Obj1");
	SideObjective1.Description = FText::FromString("붉은 약초 수집");
	SideObjective1.Type = EPGQuestObjectiveType::Collect;
	SideObjective1.TargetID = FName("Item_Red_Herb");
	SideObjective1.RequiredCount = 5;
	SideQuestData.Objectives.Add(SideObjective1);
	
	// 퀘스트 보상 설정
	FPGQuestReward SideReward1;
	SideReward1.Type = EPGQuestRewardType::Experience;
	SideReward1.RewardID = NAME_None;
	SideReward1.Amount = 50;
	SideReward1.Description = FText::FromString("경험치 50 획득");
	SideQuestData.Rewards.Add(SideReward1);
	
	FPGQuestReward SideReward2;
	SideReward2.Type = EPGQuestRewardType::Currency;
	SideReward2.RewardID = NAME_None;
	SideReward2.Amount = 30;
	SideReward2.Description = FText::FromString("골드 30 획득");
	SideQuestData.Rewards.Add(SideReward2);
	
	FPGQuestReward SideReward3;
	SideReward3.Type = EPGQuestRewardType::Item;
	SideReward3.RewardID = FName("Item_Minor_Health_Potion");
	SideReward3.Amount = 2;
	SideReward3.Description = FText::FromString("소형 체력 물약 2개 획득");
	SideQuestData.Rewards.Add(SideReward3);
	
	// 활성 퀘스트 생성 및 추가
	FPGActiveQuest SideQuest(SideQuestData);
	QuestData.Add(SideQuestData.QuestID, SideQuest);
	
	return true;
}

bool UPGQuestManager::GiveQuestRewards(FName QuestID)
{
	if (QuestID == NAME_None)
	{
		return false;
	}
	
	// 퀘스트 정보 확인
	FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	if (!QuestInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 보상 지급 실패: 존재하지 않는 퀘스트 ID - %s"), *QuestID.ToString());
		return false;
	}
	
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 보상 지급: %s"), *QuestID.ToString());
	
	// 보상 지급 - 실제 구현은 게임 시스템에 따라 달라질 수 있음
	// 아이템 지급 - 인벤토리 매니저 활용
	UGameInstance* GameInstance = Cast<UGameInstance>(GetOuter());
	if (GameInstance)
	{
		UPGManagerSubsystem* ManagerSubsystem = GameInstance->GetSubsystem<UPGManagerSubsystem>();
		if (ManagerSubsystem)
		{
			UPGInventoryManager* InventoryManager = ManagerSubsystem->GetManager<UPGInventoryManager>();
			if (InventoryManager)
			{
				for (const FPGQuestReward& Reward : QuestInfo->QuestData.Rewards)
				{
					if (Reward.Type == EPGQuestRewardType::Item && !Reward.RewardID.IsNone())
					{
						UE_LOG(LogTemp, Log, TEXT("[QuestManager] 아이템 보상: %s x%d"), *Reward.RewardID.ToString(), Reward.Amount);
						InventoryManager->AddItem(Reward.RewardID, Reward.Amount);
					}
					else if (Reward.Type == EPGQuestRewardType::Currency)
					{
						UE_LOG(LogTemp, Log, TEXT("[QuestManager] 화폐 보상: %d"), Reward.Amount);
						// 화폐 시스템 구현 필요
					}
					else if (Reward.Type == EPGQuestRewardType::Experience)
					{
						UE_LOG(LogTemp, Log, TEXT("[QuestManager] 경험치 보상: %d"), Reward.Amount);
						// 경험치 시스템 구현 필요
					}
				}
			}
		}
	}
	
	// 보상 획득 이벤트 발생
	OnQuestRewardClaimed.Broadcast(QuestID, QuestInfo->QuestData.Rewards);
	
	return true;
}

bool UPGQuestManager::SetQuestStatus(FName QuestID, EPGQuestState NewState)
{
	if (QuestID == NAME_None)
	{
		return false;
	}
	
	// 퀘스트 정보 확인
	FPGActiveQuest* QuestInfo = QuestData.Find(QuestID);
	if (!QuestInfo)
	{
		UE_LOG(LogTemp, Warning, TEXT("[QuestManager] 퀘스트 상태 변경 실패: 존재하지 않는 퀘스트 ID - %s"), *QuestID.ToString());
		return false;
	}
	
	// 상태 변경
	EPGQuestState OldState = QuestInfo->State;
	QuestInfo->State = NewState;
	
	// 상태 변경 이벤트 브로드캐스트
	OnQuestStatusChanged.Broadcast(QuestID);
	
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 상태 변경: %s, %d -> %d"), 
		*QuestID.ToString(), static_cast<int32>(OldState), static_cast<int32>(NewState));
	
	return true;
}

bool UPGQuestManager::SaveData_Implementation(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	// 활성 퀘스트 데이터 저장
	TMap<FName, FPGActiveQuest> ActiveQuestData;
	for (const FName& QuestID : ActiveQuests)
	{
		if (FPGActiveQuest* QuestInfo = QuestData.Find(QuestID))
		{
			ActiveQuestData.Add(QuestID, *QuestInfo);
		}
	}
	SaveGame->ActiveQuests = ActiveQuestData;
	
	// 완료된 퀘스트 목록 저장
	SaveGame->CompletedQuests = CompletedQuests;
	
	// 실패한 퀘스트 목록 저장
	SaveGame->FailedQuests = FailedQuests;
	
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 데이터 저장 완료 - 활성: %d, 완료: %d, 실패: %d"), 
		ActiveQuests.Num(), CompletedQuests.Num(), FailedQuests.Num());
	
	return true;
}

bool UPGQuestManager::LoadData_Implementation(UPGSaveGame* SaveGame)
{
	if (!SaveGame)
	{
		return false;
	}

	// 기존 데이터 초기화
	QuestData.Empty();
	ActiveQuests.Empty();
	CompletedQuests.Empty();
	FailedQuests.Empty();
	
	// 활성 퀘스트 데이터 로드
	QuestData = SaveGame->ActiveQuests;
	for (const auto& Pair : SaveGame->ActiveQuests)
	{
		if (Pair.Value.State == EPGQuestState::Active)
		{
			ActiveQuests.Add(Pair.Key);
		}
	}
	
	// 완료된 퀘스트 목록 로드
	CompletedQuests = SaveGame->CompletedQuests;
	
	// 실패한 퀘스트 목록 로드
	FailedQuests = SaveGame->FailedQuests;
	
	// 완료/실패 퀘스트 데이터는 QuestData에 포함되어 있지 않을 수 있으므로
	// 필요한 경우 별도로 추가하거나 다른 방식으로 처리 필요
	
	UE_LOG(LogTemp, Log, TEXT("[QuestManager] 퀘스트 데이터 로드 완료 - 활성: %d, 완료: %d, 실패: %d"), 
		ActiveQuests.Num(), CompletedQuests.Num(), FailedQuests.Num());
	
	return true;
}
