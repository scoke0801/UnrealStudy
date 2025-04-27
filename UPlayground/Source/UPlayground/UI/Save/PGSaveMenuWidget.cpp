// Copyright (c) 2025. All Rights Reserved.

#include "PGSaveMenuWidget.h"
#include "PGSaveSlotWidget.h"
#include "../../Managers/PGSaveManager.h"
#include "../../Managers/PGManagerSubsystem.h"
#include "Components/VerticalBox.h"

void UPGSaveMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 저장 매니저 가져오기
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (UPGManagerSubsystem* ManagerSubsystem = GameInstance->GetSubsystem<UPGManagerSubsystem>())
			{
				SaveManager = ManagerSubsystem->GetManager<UPGSaveManager>();
				
				if (SaveManager)
				{
					// 이벤트 바인딩
					SaveManager->OnSaveCompleted.AddDynamic(this, &UPGSaveMenuWidget::HandleSaveCompleted);
					SaveManager->OnLoadCompleted.AddDynamic(this, &UPGSaveMenuWidget::HandleLoadCompleted);
					SaveManager->OnSaveDeleted.AddDynamic(this, &UPGSaveMenuWidget::HandleSaveDeleted);
				}
			}
		}
	}

	// 기본값 설정
	if (MaxSaveSlots <= 0)
	{
		MaxSaveSlots = 10;
	}
}

void UPGSaveMenuWidget::NativeDestruct()
{
	if (SaveManager)
	{
		// 이벤트 언바인딩
		SaveManager->OnSaveCompleted.RemoveDynamic(this, &UPGSaveMenuWidget::HandleSaveCompleted);
		SaveManager->OnLoadCompleted.RemoveDynamic(this, &UPGSaveMenuWidget::HandleLoadCompleted);
		SaveManager->OnSaveDeleted.RemoveDynamic(this, &UPGSaveMenuWidget::HandleSaveDeleted);
	}

	Super::NativeDestruct();
}

UWidget* UPGSaveMenuWidget::NativeGetDesiredFocusTarget() const
{
	// 첫 번째 슬롯 위젯에 포커스 설정
	if (SlotWidgets.Num() > 0)
	{
		return SlotWidgets[0];
	}
	return Super::NativeGetDesiredFocusTarget();
}

void UPGSaveMenuWidget::OpenSaveMode()
{
	bIsSaveMode = true;
	OnModeChanged(true);
	RefreshSaveSlots();
}

void UPGSaveMenuWidget::OpenLoadMode()
{
	bIsSaveMode = false;
	OnModeChanged(false);
	RefreshSaveSlots();
}

void UPGSaveMenuWidget::RefreshSaveSlots()
{
	if (!SlotContainer || !SaveManager || !SlotWidgetClass)
	{
		return;
	}

	// 기존 슬롯 위젯 제거
	SlotContainer->ClearChildren();
	SlotWidgets.Empty();

	// 모든 저장 슬롯 가져오기
	TArray<FPGSaveMetaData> SaveSlots = SaveManager->GetAllSaveSlots();

	// 기존 저장 슬롯 표시
	for (const FPGSaveMetaData& MetaData : SaveSlots)
	{
		if (UPGSaveSlotWidget* SlotWidget = CreateWidget<UPGSaveSlotWidget>(this, SlotWidgetClass))
		{
			SlotWidget->SetSlotData(MetaData);
			SlotWidget->OnSlotSelected.AddUObject(this, &UPGSaveMenuWidget::HandleSlotSelected, SlotWidget);
			SlotWidget->OnSlotDeleteRequested.AddUObject(this, &UPGSaveMenuWidget::HandleSlotDeleteRequested, SlotWidget);
			
			SlotContainer->AddChild(SlotWidget);
			SlotWidgets.Add(SlotWidget);
		}
	}

	// 저장 모드인 경우 빈 슬롯 추가
	if (bIsSaveMode)
	{
		int32 EmptySlotsNeeded = MaxSaveSlots - SaveSlots.Num();
		for (int32 i = 0; i < EmptySlotsNeeded; ++i)
		{
			if (UPGSaveSlotWidget* SlotWidget = CreateWidget<UPGSaveSlotWidget>(this, SlotWidgetClass))
			{
				FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SaveSlots.Num() + i + 1);
				SlotWidget->SetEmptySlot(SlotName);
				SlotWidget->OnSlotSelected.AddUObject(this, &UPGSaveMenuWidget::HandleSlotSelected, SlotWidget);
				
				SlotContainer->AddChild(SlotWidget);
				SlotWidgets.Add(SlotWidget);
			}
		}
	}
}

UPGSaveSlotWidget* UPGSaveMenuWidget::CreateSlotWidget(const FPGSaveMetaData& MetaData)
{
	if (!SlotWidgetClass)
	{
		return nullptr;
	}

	UPGSaveSlotWidget* SlotWidget = CreateWidget<UPGSaveSlotWidget>(this, SlotWidgetClass);
	if (SlotWidget)
	{
		SlotWidget->SetSlotData(MetaData);
	}
	return SlotWidget;
}

UPGSaveSlotWidget* UPGSaveMenuWidget::CreateEmptySlotWidget(int32 SlotIndex)
{
	if (!SlotWidgetClass)
	{
		return nullptr;
	}

	UPGSaveSlotWidget* SlotWidget = CreateWidget<UPGSaveSlotWidget>(this, SlotWidgetClass);
	if (SlotWidget)
	{
		FString SlotName = FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
		SlotWidget->SetEmptySlot(SlotName);
	}
	return SlotWidget;
}

void UPGSaveMenuWidget::HandleSlotSelected(UPGSaveSlotWidget* SelectedSlot)
{
	if (!SelectedSlot || !SaveManager)
	{
		return;
	}

	const FPGSaveMetaData& MetaData = SelectedSlot->GetSlotData();
	
	if (bIsSaveMode)
	{
		// 저장 모드
		if (SaveManager->DoesSaveSlotExist(MetaData.SlotName))
		{
			// 덮어쓰기 확인
			ShowOverwriteConfirmation(MetaData.SlotName);
		}
		else
		{
			// 새로운 저장
			PerformSave(MetaData.SlotName);
		}
	}
	else
	{
		// 불러오기 모드
		PerformLoad(MetaData.SlotName);
	}
}

void UPGSaveMenuWidget::HandleSlotDeleteRequested(UPGSaveSlotWidget* SlotToDelete)
{
	if (!SlotToDelete)
	{
		return;
	}

	const FPGSaveMetaData& MetaData = SlotToDelete->GetSlotData();
	ShowDeleteConfirmation(MetaData.SlotName);
}

void UPGSaveMenuWidget::PerformSave(const FString& SlotName)
{
	if (!SaveManager)
	{
		return;
	}

	bool bSuccess = SaveManager->SaveGame(SlotName);
	if (bSuccess)
	{
		RefreshSaveSlots();
	}
}

void UPGSaveMenuWidget::PerformLoad(const FString& SlotName)
{
	if (!SaveManager)
	{
		return;
	}

	bool bSuccess = SaveManager->LoadGame(SlotName);
	if (bSuccess)
	{
		// 불러오기 성공 시 UI 닫기
		DeactivateWidget();
	}
}

void UPGSaveMenuWidget::HandleSaveCompleted(const FString& SlotName, bool bSuccess)
{
	if (bSuccess)
	{
		ShowResultMessage(true, FString::Printf(TEXT("게임 저장 완료: %s"), *SlotName));
	}
	else
	{
		ShowResultMessage(false, FString::Printf(TEXT("게임 저장 실패: %s"), *SlotName));
	}
}

void UPGSaveMenuWidget::HandleLoadCompleted(const FString& SlotName, bool bSuccess)
{
	if (bSuccess)
	{
		ShowResultMessage(true, FString::Printf(TEXT("게임 불러오기 완료: %s"), *SlotName));
	}
	else
	{
		ShowResultMessage(false, FString::Printf(TEXT("게임 불러오기 실패: %s"), *SlotName));
	}
}

void UPGSaveMenuWidget::HandleSaveDeleted(const FString& SlotName)
{
	ShowResultMessage(true, FString::Printf(TEXT("저장 슬롯 삭제됨: %s"), *SlotName));
	RefreshSaveSlots();
}
