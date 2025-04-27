// Copyright (c) 2025. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "../../Managers/PGSaveTypes.h"
#include "PGSaveMenuWidget.generated.h"

class UVerticalBox;
class UPGSaveSlotWidget;
class UPGSaveManager;

/**
 * 저장/불러오기 메뉴 위젯
 * 전체 저장/불러오기 UI를 관리하는 메인 위젯입니다.
 */
UCLASS()
class UPLAYGROUND_API UPGSaveMenuWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	/**
	 * 저장 모드로 메뉴 열기
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Menu")
	void OpenSaveMode();

	/**
	 * 불러오기 모드로 메뉴 열기
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Menu")
	void OpenLoadMode();

protected:
	// Native event handlers
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

	/**
	 * 모드 변경
	 * @param bSaveMode 저장 모드 여부
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Save Menu")
	void OnModeChanged(bool bSaveMode);

	/**
	 * 저장 슬롯 목록 리프레시
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Menu")
	void RefreshSaveSlots();

	/**
	 * 슬롯 위젯 생성
	 * @param MetaData 저장 슬롯 메타데이터
	 * @return 생성된 슬롯 위젯
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Menu")
	UPGSaveSlotWidget* CreateSlotWidget(const FPGSaveMetaData& MetaData);

	/**
	 * 빈 슬롯 위젯 생성
	 * @param SlotIndex 슬롯 인덱스
	 * @return 생성된 슬롯 위젯
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Menu")
	UPGSaveSlotWidget* CreateEmptySlotWidget(int32 SlotIndex);

	/**
	 * 슬롯 선택 핸들러
	 */
	void HandleSlotSelected(UPGSaveSlotWidget* SelectedSlot);

	/**
	 * 슬롯 삭제 요청 핸들러
	 */
	void HandleSlotDeleteRequested(UPGSaveSlotWidget* SlotToDelete);

	/**
	 * 저장 수행
	 * @param SlotName 저장할 슬롯 이름
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Menu")
	void PerformSave(const FString& SlotName);

	/**
	 * 불러오기 수행
	 * @param SlotName 불러올 슬롯 이름
	 */
	UFUNCTION(BlueprintCallable, Category = "Save Menu")
	void PerformLoad(const FString& SlotName);

	/**
	 * 삭제 확인 팝업 표시
	 * @param SlotName 삭제할 슬롯 이름
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Save Menu")
	void ShowDeleteConfirmation(const FString& SlotName);

	/**
	 * 덮어쓰기 확인 팝업 표시
	 * @param SlotName 덮어쓸 슬롯 이름
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Save Menu")
	void ShowOverwriteConfirmation(const FString& SlotName);

	/**
	 * 저장/불러오기 결과 표시
	 * @param bSuccess 성공 여부
	 * @param Message 메시지
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Save Menu")
	void ShowResultMessage(bool bSuccess, const FString& Message);

	// 세이브 매니저 이벤트 핸들러
	UFUNCTION()
	void HandleSaveCompleted(const FString& SlotName, bool bSuccess);

	UFUNCTION()
	void HandleLoadCompleted(const FString& SlotName, bool bSuccess);

	UFUNCTION()
	void HandleSaveDeleted(const FString& SlotName);

protected:
	// 저장 슬롯 컨테이너
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UVerticalBox* SlotContainer;

	// 슬롯 위젯 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Save Menu")
	TSubclassOf<UPGSaveSlotWidget> SlotWidgetClass;

	// 최대 저장 슬롯 수
	UPROPERTY(EditDefaultsOnly, Category = "Save Menu")
	int32 MaxSaveSlots;

	// 현재 저장 모드
	UPROPERTY(BlueprintReadOnly, Category = "Save Menu")
	bool bIsSaveMode;

	// 저장 매니저
	UPROPERTY(BlueprintReadOnly, Category = "Save Menu")
	UPGSaveManager* SaveManager;

	// 생성된 슬롯 위젯 목록
	UPROPERTY()
	TArray<UPGSaveSlotWidget*> SlotWidgets;
};
