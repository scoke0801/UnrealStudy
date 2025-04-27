// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PGSaveTypes.generated.h"

/**
 * 저장 슬롯 타입
 * 저장 슬롯의 종류를 정의합니다.
 */
UENUM(BlueprintType)
enum class EPGSaveSlotType : uint8
{
	Manual      UMETA(DisplayName = "Manual Save"),
	Auto        UMETA(DisplayName = "Auto Save"),
	Quick       UMETA(DisplayName = "Quick Save"),
	Checkpoint  UMETA(DisplayName = "Checkpoint Save")
};

/**
 * 저장 메타데이터 구조체
 * 저장 슬롯의 메타데이터를 관리합니다.
 */
USTRUCT(BlueprintType)
struct FPGSaveMetaData
{
	GENERATED_BODY()

	// 저장 슬롯 이름
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	FString SlotName;

	// 저장 슬롯 타입
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	EPGSaveSlotType SlotType;

	// 캐릭터 이름
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	FString CharacterName;

	// 캐릭터 레벨
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	int32 CharacterLevel;

	// 플레이 시간 (초)
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	float PlayTime;

	// 저장 시간
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	FDateTime SaveDateTime;

	// 현재 레벨 (맵 이름)
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	FString CurrentLevel;

	// 썸네일 이미지 경로
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	FString ThumbnailPath;

	// 저장 위치 (저장 시점의 위치)
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	FVector SaveLocation;

	// 저장 회전값 (저장 시점의 회전값)
	UPROPERTY(BlueprintReadWrite, Category = "Save Meta")
	FRotator SaveRotation;

	// 기본 생성자
	FPGSaveMetaData()
		: SlotName("")
		, SlotType(EPGSaveSlotType::Manual)
		, CharacterName("")
		, CharacterLevel(1)
		, PlayTime(0.0f)
		, SaveDateTime(FDateTime::Now())
		, CurrentLevel("")
		, ThumbnailPath("")
		, SaveLocation(FVector::ZeroVector)
		, SaveRotation(FRotator::ZeroRotator)
	{
	}
};

/**
 * 자동 저장 설정 구조체
 */
USTRUCT(BlueprintType)
struct FPGAutoSaveConfig
{
	GENERATED_BODY()

	// 자동 저장 활성화 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Save")
	bool bEnabled;

	// 자동 저장 간격 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Save")
	float Interval;

	// 최대 자동 저장 슬롯 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Save")
	int32 MaxAutoSaves;

	// 기본 생성자
	FPGAutoSaveConfig()
		: bEnabled(true)
		, Interval(300.0f)  // 5분
		, MaxAutoSaves(3)
	{
	}
};

/**
 * 저장 데이터에 대한 인터페이스
 * 저장 가능한 객체들이 구현해야 할 인터페이스입니다.
 */
UINTERFACE(MinimalAPI, Blueprintable)
class UPGSavableInterface : public UInterface
{
	GENERATED_BODY()
};

class IPGSavableInterface
{
	GENERATED_BODY()

public:
	/**
	 * 객체의 데이터를 저장합니다.
	 * @param SaveGame 저장할 SaveGame 객체
	 * @return 저장 성공 여부
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Save")
	bool SaveData(class UPGSaveGame* SaveGame);

	/**
	 * 저장된 데이터를 로드합니다.
	 * @param SaveGame 로드할 SaveGame 객체
	 * @return 로드 성공 여부
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Save")
	bool LoadData(class UPGSaveGame* SaveGame);
};

/**
 * 저장 이벤트 델리게이트
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPGSaveCompleted, const FString&, SlotName, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPGLoadCompleted, const FString&, SlotName, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPGSaveDeleted, const FString&, SlotName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPGAutoSaveTriggered);
