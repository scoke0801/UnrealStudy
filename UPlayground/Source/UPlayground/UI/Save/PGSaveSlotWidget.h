// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "../../Managers/PGSaveTypes.h"
#include "PGSaveSlotWidget.generated.h"

/**
 * 저장 슬롯 위젯
 * 개별 저장 슬롯을 표시하는 UI 위젯입니다.
 */
UCLASS()
class UPLAYGROUND_API UPGSaveSlotWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/**
	 * 슬롯 데이터 설정
	 * @param InMetaData 저장 슬롯 메타데이터
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Slot")
	void SetSlotData(const FPGSaveMetaData& InMetaData);

	/**
	 * 슬롯 데이터 가져오기
	 * @return 저장 슬롯 메타데이터
	 */
	UFUNCTION(BlueprintPure, Category = "Save Slot")
	const FPGSaveMetaData& GetSlotData() const { return MetaData; }

	/**
	 * 빈 슬롯으로 설정
	 * @param SlotName 슬롯 이름
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Slot")
	void SetEmptySlot(const FString& SlotName);

	DECLARE_MULTICAST_DELEGATE(FOnSlotSelected);
	DECLARE_MULTICAST_DELEGATE(FOnSlotDeleteRequested);

	/**
	 * 슬롯 선택 이벤트
	 */
	FOnSlotSelected OnSlotSelected;

	/**
	 * 슬롯 삭제 이벤트
	 */
	FOnSlotDeleteRequested OnSlotDeleteRequested;

protected:
	// Native event handlers
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/**
	 * 슬롯 정보 업데이트
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Save Slot")
	void OnUpdateSlotInfo();

	/**
	 * 슬롯 선택 버튼 클릭
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Slot")
	void HandleSlotClicked();

	/**
	 * 슬롯 삭제 버튼 클릭
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Slot")
	void HandleDeleteClicked();

	/**
	 * 슬롯 포맷 시간
	 * @param DateTime 시간
	 * @return 포맷된 문자열
	 */
	UFUNCTION(BlueprintPure, Category = "Save Slot")
	FString FormatDateTime(const FDateTime& DateTime) const;

	/**
	 * 플레이 시간 포맷
	 * @param Seconds 초 단위 시간
	 * @return 포맷된 문자열
	 */
	UFUNCTION(BlueprintPure, Category = "Save Slot")
	FString FormatPlayTime(float Seconds) const;

protected:
	// 저장 슬롯 메타데이터
	UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
	FPGSaveMetaData MetaData;

	// 빈 슬롯 여부
	UPROPERTY(BlueprintReadOnly, Category = "Save Slot")
	bool bIsEmptySlot;
};
