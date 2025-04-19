// Copyright Epic Games, Inc. All Rights Reserved.

#include "PGPlayerController.h"
#include "Blueprint/UserWidget.h"
// CommonUI 플러그인이 없어서 제거

APGPlayerController::APGPlayerController()
{
    // 기본 설정
}

void APGPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 로컬 플레이어인 경우에만 HUD 설정
    if (IsLocalPlayerController())
    {
        SetupHUD();
    }
}

void APGPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void APGPlayerController::SetupHUD()
{
    if (MainHUDClass && IsLocalPlayerController())
    {
        MainHUDWidget = CreateWidget<UUserWidget>(this, MainHUDClass);
        if (MainHUDWidget)
        {
            MainHUDWidget->AddToViewport();
        }
    }
}

void APGPlayerController::OpenInventory()
{
    // 인벤토리 UI 열기 로직
    SetInputModeToGameAndUI();
}

void APGPlayerController::OpenQuestLog()
{
    // 퀘스트 로그 UI 열기 로직
    SetInputModeToGameAndUI();
}

void APGPlayerController::OpenMap()
{
    // 지도 UI 열기 로직
    SetInputModeToGameAndUI();
}

void APGPlayerController::TogglePauseMenu()
{
    // 일시 정지 메뉴 토글 로직
    SetInputModeToUIOnly();
}

void APGPlayerController::ClientUpdateUI_Implementation()
{
    // 클라이언트 UI 업데이트 로직
}

void APGPlayerController::SetInputModeToGameOnly()
{
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    bShowMouseCursor = false;
}

void APGPlayerController::SetInputModeToGameAndUI()
{
    FInputModeGameAndUI InputMode;
    SetInputMode(InputMode);
    bShowMouseCursor = true;
}

void APGPlayerController::SetInputModeToUIOnly()
{
    FInputModeUIOnly InputMode;
    SetInputMode(InputMode);
    bShowMouseCursor = true;
}