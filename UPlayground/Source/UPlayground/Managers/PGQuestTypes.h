// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGQuestTypes.generated.h"

// 전방 선언
class UPGItem;

/**
 * 퀘스트 타입 열거형
 * 퀘스트의 타입을 나타냅니다.
 */
UENUM(BlueprintType)
enum class EPGQuestType : uint8
{
	Main        UMETA(DisplayName = "Main Quest"),
	Side        UMETA(DisplayName = "Side Quest"),
	Daily       UMETA(DisplayName = "Daily Quest"),
	World       UMETA(DisplayName = "World Quest"),
	Hidden      UMETA(DisplayName = "Hidden Quest")
};

/**
 * 퀘스트 상태 열거형
 * 퀘스트의 현재 상태를 나타냅니다.
 */
UENUM(BlueprintType)
enum class EPGQuestState : uint8
{
	Inactive    UMETA(DisplayName = "Inactive"),
	Active      UMETA(DisplayName = "Active"),
	Completed   UMETA(DisplayName = "Completed"),
	Failed      UMETA(DisplayName = "Failed"),
	Abandoned   UMETA(DisplayName = "Abandoned")
};

/**
 * 퀘스트 목표 타입 열거형
 * 퀘스트 목표의 타입을 나타냅니다.
 */
UENUM(BlueprintType)
enum class EPGQuestObjectiveType : uint8
{
	Kill        UMETA(DisplayName = "Kill Enemies"),
	Collect     UMETA(DisplayName = "Collect Items"),
	Interact    UMETA(DisplayName = "Interact with Objects"),
	Escort      UMETA(DisplayName = "Escort NPC"),
	Reach       UMETA(DisplayName = "Reach Location"),
	Talk        UMETA(DisplayName = "Talk to NPC"),
	Craft       UMETA(DisplayName = "Craft Items"),
	Defeat      UMETA(DisplayName = "Defeat Boss")
};

/**
 * 퀘스트 목표 데이터 구조체
 * 퀘스트의 개별 목표를 정의합니다.
 */
USTRUCT(BlueprintType)
struct FPGQuestObjective
{
	GENERATED_BODY()

	// 목표 ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	FName ObjectiveID;

	// 목표 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	FText Description;

	// 목표 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	EPGQuestObjectiveType Type;

	// 목표 대상 ID (적, 아이템, NPC 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	FName TargetID;

	// 목표 수량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	int32 RequiredCount;

	// 현재 진행 수량
	UPROPERTY(BlueprintReadOnly, Category = "Quest Objective")
	int32 CurrentCount;

	// 목표 완료 여부
	UPROPERTY(BlueprintReadOnly, Category = "Quest Objective")
	bool bIsCompleted;

	// 선택적 목표 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Objective")
	bool bIsOptional;

	// 기본 생성자
	FPGQuestObjective()
		: ObjectiveID(NAME_None)
		, Description(FText::GetEmpty())
		, Type(EPGQuestObjectiveType::Kill)
		, TargetID(NAME_None)
		, RequiredCount(1)
		, CurrentCount(0)
		, bIsCompleted(false)
		, bIsOptional(false)
	{
	}
};

/**
 * 퀘스트 보상 타입 열거형
 * 퀘스트 보상의 타입을 나타냅니다.
 */
UENUM(BlueprintType)
enum class EPGQuestRewardType : uint8
{
	Item        UMETA(DisplayName = "Item Reward"),
	Currency    UMETA(DisplayName = "Currency Reward"),
	Experience  UMETA(DisplayName = "Experience Reward"),
	Reputation  UMETA(DisplayName = "Reputation Reward"),
	Skill       UMETA(DisplayName = "Skill Reward")
};

/**
 * 퀘스트 보상 데이터 구조체
 * 퀘스트 완료 시 제공되는 보상을 정의합니다.
 */
USTRUCT(BlueprintType)
struct FPGQuestReward
{
	GENERATED_BODY()

	// 보상 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Reward")
	EPGQuestRewardType Type;

	// 보상 ID (아이템, 스킬 등)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Reward")
	FName RewardID;

	// 보상 수량
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Reward")
	int32 Amount;

	// 보상 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest Reward")
	FText Description;

	// 기본 생성자
	FPGQuestReward()
		: Type(EPGQuestRewardType::Item)
		, RewardID(NAME_None)
		, Amount(1)
		, Description(FText::GetEmpty())
	{
	}
};

/**
 * 퀘스트 데이터 구조체
 * 퀘스트의 기본 정보를 정의합니다.
 */
USTRUCT(BlueprintType)
struct FPGQuestData
{
	GENERATED_BODY()

	// 퀘스트 ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName QuestID;

	// 퀘스트 제목
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText Title;

	// 퀘스트 설명
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FText Description;

	// 퀘스트 타입
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	EPGQuestType Type;

	// 퀘스트 레벨
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	int32 Level;

	// 선행 퀘스트 ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FName> PrerequisiteQuests;

	// 다음 퀘스트 ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FName> NextQuests;

	// 퀘스트 목표 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FPGQuestObjective> Objectives;

	// 퀘스트 보상 목록
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TArray<FPGQuestReward> Rewards;

	// 시작 NPC ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName StartNPCID;

	// 완료 NPC ID
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	FName CompleteNPCID;

	// 퀘스트 아이콘
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	TSoftObjectPtr<UTexture2D> Icon;

	// 반복 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	bool bIsRepeatable;

	// 시간 제한 (초 단위, 0이면 제한 없음)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Quest")
	float TimeLimit;

	// 기본 생성자
	FPGQuestData()
		: QuestID(NAME_None)
		, Title(FText::GetEmpty())
		, Description(FText::GetEmpty())
		, Type(EPGQuestType::Side)
		, Level(1)
		, StartNPCID(NAME_None)
		, CompleteNPCID(NAME_None)
		, bIsRepeatable(false)
		, TimeLimit(0.0f)
	{
	}
};

/**
 * 활성 퀘스트 구조체
 * 현재 플레이어가 진행 중인 퀘스트의 상태를 관리합니다.
 */
USTRUCT(BlueprintType)
struct FPGActiveQuest
{
	GENERATED_BODY()

	// 퀘스트 데이터
	UPROPERTY(BlueprintReadOnly, Category = "Active Quest")
	FPGQuestData QuestData;

	// 퀘스트 상태
	UPROPERTY(BlueprintReadOnly, Category = "Active Quest")
	EPGQuestState State;

	// 목표 진행 상황
	UPROPERTY(BlueprintReadOnly, Category = "Active Quest")
	TArray<FPGQuestObjective> Objectives;

	// 시작 시간
	UPROPERTY(BlueprintReadOnly, Category = "Active Quest")
	FDateTime StartTime;

	// 만료 시간 (TimeLimit이 있는 경우)
	UPROPERTY(BlueprintReadOnly, Category = "Active Quest")
	FDateTime ExpiryTime;

	// 기본 생성자
	FPGActiveQuest()
		: State(EPGQuestState::Inactive)
		, StartTime(FDateTime::Now())
		, ExpiryTime(FDateTime::Now())
	{
	}

	// 매개변수 생성자
	FPGActiveQuest(const FPGQuestData& InQuestData)
		: QuestData(InQuestData)
		, State(EPGQuestState::Active)
		, Objectives(InQuestData.Objectives)
		, StartTime(FDateTime::Now())
	{
		// 시간 제한이 있는 경우 만료 시간 설정
		if (InQuestData.TimeLimit > 0.0f)
		{
			ExpiryTime = StartTime + FTimespan::FromSeconds(InQuestData.TimeLimit);
		}
		else
		{
			// 시간 제한이 없는 경우 최대값으로 설정
			ExpiryTime = FDateTime::MaxValue();
		}
	}

	/**
	 * 퀘스트 목표 진행 함수
	 * @param ObjectiveID 진행할 목표 ID
	 * @param Count 진행 수량
	 * @return 목표 완료 여부
	 */
	bool ProgressObjective(FName ObjectiveID, int32 Count = 1)
	{
		for (FPGQuestObjective& Objective : Objectives)
		{
			if (Objective.ObjectiveID == ObjectiveID && !Objective.bIsCompleted)
			{
				Objective.CurrentCount += Count;
				
				// 목표 완료 여부 확인
				if (Objective.CurrentCount >= Objective.RequiredCount)
				{
					Objective.CurrentCount = Objective.RequiredCount;
					Objective.bIsCompleted = true;
				}
				
				return Objective.bIsCompleted;
			}
		}
		
		return false;
	}

	/**
	 * 모든 목표가 완료되었는지 확인
	 * @return 모든 필수 목표 완료 여부
	 */
	bool AreAllObjectivesCompleted() const
	{
		for (const FPGQuestObjective& Objective : Objectives)
		{
			// 선택적 목표가 아니고 완료되지 않은 경우
			if (!Objective.bIsOptional && !Objective.bIsCompleted)
			{
				return false;
			}
		}
		
		return true;
	}

	/**
	 * 퀘스트 만료 여부 확인
	 * @return 퀘스트 만료 여부
	 */
	bool IsExpired() const
	{
		return QuestData.TimeLimit > 0.0f && ExpiryTime <= FDateTime::Now();
	}
};

/**
 * 퀘스트 이벤트 델리게이트
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPGQuestStatusChanged, FName, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPGQuestObjectiveUpdated, FName, QuestID, FName, ObjectiveID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPGQuestCompleted, FName, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPGQuestFailed, FName, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPGQuestRewardClaimed, FName, QuestID, const TArray<FPGQuestReward>&, Rewards);
