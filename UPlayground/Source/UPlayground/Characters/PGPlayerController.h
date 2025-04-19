// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PGPlayerController.generated.h"

UCLASS()
class UPLAYGROUND_API APGPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    APGPlayerController();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

protected:
    // UI 관련 속성
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UUserWidget> MainHUDClass;

    UPROPERTY(BlueprintReadOnly, Category = "UI")
    class UUserWidget* MainHUDWidget;

    // 게임 플레이 관련 메서드
    UFUNCTION(BlueprintCallable, Category = "Player Controller")
    virtual void SetupHUD();

    UFUNCTION(BlueprintCallable, Category = "Player Controller")
    virtual void OpenInventory();

    UFUNCTION(BlueprintCallable, Category = "Player Controller")
    virtual void OpenQuestLog();

    UFUNCTION(BlueprintCallable, Category = "Player Controller")
    virtual void OpenMap();

    UFUNCTION(BlueprintCallable, Category = "Player Controller")
    virtual void TogglePauseMenu();

    // 네트워크 관련 메서드
    UFUNCTION(Client, Reliable)
    void ClientUpdateUI();

private:
    // 입력 모드 전환 헬퍼 함수
    void SetInputModeToGameOnly();
    void SetInputModeToGameAndUI();
    void SetInputModeToUIOnly();
};